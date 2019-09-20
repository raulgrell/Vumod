#include "StringUtils.h"

#include <algorithm>
#include <sstream>

namespace str
{
    std::vector<std::string> split(const std::string &strToSplit, char delimeter)
    {
        std::vector<std::string> result;
        std::stringstream ss(strToSplit);
        std::string item;
        while (std::getline(ss, item, delimeter))
            result.push_back(item);
        return result;
    }

    std::string trimmed(const std::string &str)
    {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos)
            return str;
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
}
