#include "common.h"

static void fatal_error(std::string errorString)
{
    perror(errorString.c_str());
    glfwTerminate();
    system("PAUSE");
    exit(1);
}

static std::vector<std::string> split(std::string strToSplit, char delimeter)
{
    std::vector<std::string> result;
    std::stringstream ss(strToSplit);
    std::string item;
    while (std::getline(ss, item, delimeter))
    {
       result.push_back(item);
    }
    return result;
}