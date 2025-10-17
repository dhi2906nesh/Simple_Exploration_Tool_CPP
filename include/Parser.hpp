#pragma once
#include <string>
#include <vector>

struct LoopNode {
    int id;                     // unique id
    std::string label;          // loop header or statement
    std::vector<int> children;  // ids of child nodes
    int parent;                 // parent id (-1 for root)
};

class Parser {
public:
    // Construct with raw source code (already loaded)
    explicit Parser(const std::string& code);

    // Parse loops and return vector of nodes (forest roots are parent == -1)
    std::vector<LoopNode> parse();

private:
    std::string code_;
    std::vector<LoopNode> nodes_;

    // helpers
    void removeComments();
    std::vector<std::string> tokenizeLinesPreserveBlocks() const;
    int addNode(const std::string& label, int parent);
};
