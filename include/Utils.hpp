#pragma once
#include <string>

namespace Utils {
    std::string readFile(const std::string& path);
    std::string trim(const std::string& s);
    std::string escapeDotLabel(const std::string& s);
}
