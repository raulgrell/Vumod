#pragma once

#include "common.h"

struct VuScript
{
    VuScript();
    ~VuScript();

    void InterpretCommands();

    static std::vector<std::string> commands;
    static char command_text[4096];
};
