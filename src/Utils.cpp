#include "Utils.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace Utils {

std::string readFile(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) return {};
    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end-1]))) --end;
    return s.substr(start, end - start);
}

std::string escapeDotLabel(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 10);
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\t': out += "\\t"; break;
            case '<': out += "\\<"; break;
            case '>': out += "\\>"; break;
            case '\\': out += "\\\\"; break;
            default: out += c;
        }
    }
    return out;
}

} // namespace Utils
