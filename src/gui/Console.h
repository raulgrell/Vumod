#pragma once

#include <imgui.h>
#include <util/StringUtils.h>
#include "common.h"

struct Console
{
    char inputBuf[256] {0};
    std::vector<std::string> items;
    std::vector<std::string> history;
    int historyPos = -1;
    bool autoScroll = true;
    bool scrollToBottom = true;

    Console();

    void ClearLog();
    void AddLog(const char *fmt, ...) IM_FMTARGS(2);
    void Draw(const char *title, bool *pOpen);
    void Execute(const char *commandLine);
    int TextEditCallback(ImGuiInputTextCallbackData *data);
};
