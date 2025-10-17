#include "Parser.hpp"
#include "Utils.hpp"
#include <regex>
#include <sstream>
#include <iostream>

Parser::Parser(const std::string& code) : code_(code) {}

int Parser::addNode(const std::string& label, int parent) {
    LoopNode node;
    node.id = static_cast<int>(nodes_.size()) + 1; // 1-based ids
    node.label = label;
    node.parent = parent;
    nodes_.push_back(std::move(node));
    return nodes_.back().id;
}

void Parser::removeComments() {
    // Remove // comments
    std::string tmp;
    tmp.reserve(code_.size());
    bool in_single = false, in_multi = false;

    for (size_t i = 0; i < code_.size(); ++i) {
        if (!in_single && !in_multi && i + 1 < code_.size() && code_[i] == '/' && code_[i+1] == '/') {
            in_single = true;
            ++i; // skip next char
            continue;
        }
        if (!in_single && !in_multi && i + 1 < code_.size() && code_[i] == '/' && code_[i+1] == '*') {
            in_multi = true;
            ++i;
            continue;
        }
        if (in_single && code_[i] == '\n') {
            in_single = false;
            tmp += '\n';
            continue;
        }
        if (in_multi && i + 1 < code_.size() && code_[i] == '*' && code_[i+1] == '/') {
            in_multi = false;
            ++i;
            continue;
        }
        if (!in_single && !in_multi) tmp += code_[i];
    }
    code_.swap(tmp);
}

std::vector<std::string> Parser::tokenizeLinesPreserveBlocks() const {
    // We want a list of "logical lines" where loop headers that span lines are combined
    std::vector<std::string> lines;
    std::istringstream ss(code_);
    std::string raw;
    std::string acc;
    int parenDepth = 0;

    while (std::getline(ss, raw)) {
        std::string rtrim = Utils::trim(raw);
        if (rtrim.empty()) {
            if (!acc.empty()) acc += '\n';
            continue;
        }

        // accumulate and track parentheses to allow multiline for(...) or while(...)
        for (char c : rtrim) {
            if (c == '(') ++parenDepth;
            else if (c == ')') --parenDepth;
        }

        if (acc.empty()) acc = rtrim;
        else acc += " " + rtrim;

        // end accumulation if parentheses balanced
        if (parenDepth <= 0) {
            lines.push_back(acc);
            acc.clear();
            parenDepth = 0;
        } else {
            // keep accumulating
        }
    }
    if (!acc.empty()) lines.push_back(acc);
    return lines;
}

std::vector<LoopNode> Parser::parse() {
    nodes_.clear();
    removeComments();

    std::vector<std::string> lines = tokenizeLinesPreserveBlocks();

    // We'll keep a stack of node ids representing current nesting
    std::vector<int> stack; // stores node ids
    // Keep track of brace depth per line using scanning of '{' and '}'
    std::regex forRegex(R"(\bfor\s*\()");     // matches 'for(' with optional spaces
    std::regex whileRegex(R"(\bwhile\s*\()"); // matches 'while('
    std::regex doRegex(R"(\bdo\b)");         // not implemented fully

    int braceDepth = 0;
    for (const auto& line : lines) {
        std::string s = Utils::trim(line);
        if (s.empty()) continue;

        bool isLoop = false;
        std::smatch m;

        if (std::regex_search(s, forRegex)) {
            // Capture full header up to first { if present
            std::string header = s;
            // remove trailing { if present to keep header clean
            size_t pos = header.find('{');
            if (pos != std::string::npos) header = Utils::trim(header.substr(0, pos));
            int parent = stack.empty() ? -1 : stack.back();
            int id = addNode(header, parent);
            if (parent != -1) {
                // attach to parent
                auto &pnode = nodes_[parent - 1];
                pnode.children.push_back(id);
            }
            else {
                // root node - parent == -1
            }
            // Push the new loop node onto stack only if it opens a brace later or current line has '{'
            stack.push_back(id);
            isLoop = true;
        } else if (std::regex_search(s, whileRegex)) {
            // while loop: could be a closing while of do-while; we treat it as loop header
            std::string header = s;
            size_t pos = header.find('{');
            if (pos != std::string::npos) header = Utils::trim(header.substr(0, pos));
            int parent = stack.empty() ? -1 : stack.back();
            int id = addNode(header, parent);
            if (parent != -1) nodes_[parent - 1].children.push_back(id);
            stack.push_back(id);
            isLoop = true;
        } else {
            // not a loop header - but check for simple statements inside current scope
            // We only record statements if they are direct children of the top loop and are not braces-only lines
            std::string tmp = s;
            // ignore isolated '{' or '}' and else/if/for/while labels handled above
            if (tmp != "{" && tmp != "}" && tmp != ";") {
                // treat as statement
                if (!stack.empty()) {
                    int parent = stack.back();
                    int id = addNode(tmp, parent);
                    nodes_[parent - 1].children.push_back(id);
                    // we do NOT push statements to stack
                } else {
                    // top-level statements — keep as root nodes too
                    int id = addNode(tmp, -1);
                    (void)id;
                }
            }
        }

        // Now track braces in the original line to know when to pop stack
        for (char c : line) {
            if (c == '{') {
                ++braceDepth;
            } else if (c == '}') {
                --braceDepth;
                // when a closing brace appears, pop one scope if any loop is waiting
                if (!stack.empty()) {
                    // Pop last loop node only if its scope has closed.
                    // It's approximate because we don't track explicit per-node brace count,
                    // but since we push when we see loop header and encounter braces in code order,
                    // popping on '}' will correctly reduce nesting for typical well-formed code.
                    stack.pop_back();
                }
            }
        }
    }

    return nodes_;
}
