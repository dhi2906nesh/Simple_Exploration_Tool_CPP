#include <iostream>
#include <string>
#include <vector>
#include "Parser.hpp"
#include "Visualizer.hpp"
#include "Utils.hpp"

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <source.cpp> [--ascii] [--dot output.dot] [--save-ascii out.txt]\n";
    std::cout << "Defaults: prints ASCII to stdout. Use --dot to write Graphviz DOT.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string path = argv[1];
    bool askAscii = true;
    std::string dotPath;
    std::string asciiOut;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ascii") askAscii = true;
        else if (arg == "--dot") {
            if (i + 1 < argc) {
                dotPath = argv[++i];
            } else {
                std::cerr << "--dot requires an argument\n";
                return 1;
            }
        } else if (arg == "--save-ascii") {
            if (i + 1 < argc) {
                asciiOut = argv[++i];
            } else {
                std::cerr << "--save-ascii requires a filename\n";
                return 1;
            }
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    std::string code = Utils::readFile(path);
    if (code.empty()) {
        std::cerr << "Failed to read file or file empty: " << path << "\n";
        return 1;
    }

    Parser parser(code);
    auto nodes = parser.parse();

    if (askAscii) {
        Visualizer::printAscii(nodes);
    }

    if (!asciiOut.empty()) {
        if (!Visualizer::saveAscii(nodes, asciiOut)) {
            std::cerr << "Failed to save ASCII to " << asciiOut << "\n";
        } else {
            std::cerr << "Saved ASCII to " << asciiOut << "\n";
        }
    }

    if (!dotPath.empty()) {
        if (!Visualizer::generateDot(nodes, dotPath)) {
            std::cerr << "Failed to write DOT to " << dotPath << "\n";
            return 1;
        } else {
            std::cerr << "Wrote DOT to " << dotPath << "\n";
        }
    }

    return 0;
}
