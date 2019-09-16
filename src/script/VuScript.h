#pragma once

#include <vector>
#include <string>

struct VuScript
{
    VuScript();
    ~VuScript();

    void InterpretCommands();

    static std::vector<std::string> commands;
    static char command_text[4096];
};
