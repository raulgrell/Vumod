#include "common.h"

static void fatal_error(const std::string &errorString)
{
    perror(errorString.c_str());
    glfwTerminate();
    system("PAUSE");
    exit(1);
}

static std::vector<std::string> splitString(const std::string &strToSplit, char delimeter)
{
    std::vector<std::string> result;
    std::stringstream ss(strToSplit);
    std::string item;
    while (std::getline(ss, item, delimeter))
       result.push_back(item);
    return result;
}

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

static std::string trimmed(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}