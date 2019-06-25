#include <vector>
#include <string>

std::vector<std::string> splitString(const std::string &strToSplit, char delimeter);
void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

void fatal_error(const std::string &errorString);
