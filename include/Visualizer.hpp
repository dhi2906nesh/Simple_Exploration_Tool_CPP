#pragma once
#include <string>
#include <vector>
#include "Parser.hpp"

namespace Visualizer {
    // Print ASCII tree of loop nodes to stdout
    void printAscii(const std::vector<LoopNode>& nodes);

    // Generate Graphviz DOT file
    bool generateDot(const std::vector<LoopNode>& nodes, const std::string& outPath);

    // Save ASCII to file (optional helper)
    bool saveAscii(const std::vector<LoopNode>& nodes, const std::string& outPath);
}
