#include <vector>
#include <string>

namespace str {
    std::vector<std::string> split(const std::string &strToSplit, char delimeter);
    void ltrim(std::string &s);
    void rtrim(std::string &s);
    void trim(std::string &s);
}
