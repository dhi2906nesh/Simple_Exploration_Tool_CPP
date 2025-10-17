#include "Visualizer.hpp"
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>

namespace Visualizer {

static void printNodeAscii(const std::vector<LoopNode>& nodes, int nodeId, const std::string& prefix, bool isLast) {
    const LoopNode& node = nodes[nodeId - 1];
    std::cout << prefix;
    if (!prefix.empty()) std::cout << (isLast ? "└── " : "├── ");
    std::cout << node.label << "\n";

    std::string childPrefix = prefix + (isLast ? "    " : "│   ");
    for (size_t i = 0; i < node.children.size(); ++i) {
        bool last = (i + 1 == node.children.size());
        printNodeAscii(nodes, node.children[i], childPrefix, last);
    }
}

void printAscii(const std::vector<LoopNode>& nodes) {
    // find roots (parent == -1)
    std::vector<int> roots;
    for (const auto& n : nodes) if (n.parent == -1) roots.push_back(n.id);

    if (roots.empty()) {
        std::cout << "[no loops / nodes found]\n";
        return;
    }

    for (size_t i = 0; i < roots.size(); ++i) {
        bool last = (i + 1 == roots.size());
        printNodeAscii(nodes, roots[i], "", last);
    }
}

bool generateDot(const std::vector<LoopNode>& nodes, const std::string& outPath) {
    std::ofstream ofs(outPath);
    if (!ofs) return false;
    ofs << "digraph LoopStructure {\n";
    ofs << "  node [shape=box, fontname=\"Courier New\"];\n";

    // Write nodes
    for (const auto& n : nodes) {
        ofs << "  n" << n.id << " [label=\"" << Utils::escapeDotLabel(n.label) << "\"];\n";
    }
    // Write edges
    for (const auto& n : nodes) {
        for (int child : n.children) {
            ofs << "  n" << n.id << " -> n" << child << ";\n";
        }
    }
    ofs << "}\n";
    ofs.close();
    return true;
}

bool saveAscii(const std::vector<LoopNode>& nodes, const std::string& outPath) {
    std::ofstream ofs(outPath);
    if (!ofs) return false;
    // capture stdout-like content into ofs
    std::streambuf* coutbuf = std::cout.rdbuf(); // save
    std::cout.rdbuf(ofs.rdbuf());
    printAscii(nodes);
    std::cout.rdbuf(coutbuf); // restore
    ofs.close();
    return true;
}

} // namespace Visualizer
