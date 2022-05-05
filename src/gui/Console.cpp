
#include <util/StringUtils.h>
#include "Console.h"
#include "common.h"

Console::Console()
{
    ClearLog();
    AddLog("Welcome to Dear ImGui!");
}

void Console::ClearLog()
{
    items.clear();
    scrollToBottom = true;
}

void Console::AddLog(const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);
    items.emplace_back(buf);
    if (autoScroll)
        scrollToBottom = true;
}

void Console::Draw(const char *title, bool *pOpen)
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, pOpen)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Close Console"))
            *pOpen = false;
        ImGui::EndPopup();
    }

    ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

    ImGui::SameLine();
    if (ImGui::SmallButton("Clear"))
        ClearLog();

    ImGui::SameLine();
    bool copyToClipboard = ImGui::SmallButton("Copy");

    ImGui::SameLine();
    if (ImGui::SmallButton("Scroll to bottom"))
        scrollToBottom = true;

    ImGui::Separator();

    // 1 separator, 1 input text
    float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Clear"))
            ClearLog();
        ImGui::EndPopup();
    }

    // TODO: display items starting from the bottom
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    if (copyToClipboard)
        ImGui::LogToClipboard();

    for (auto &i : items) {
        const char *item = i.c_str();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
        ImGui::TextUnformatted(item);
        ImGui::PopStyleColor();
    }

    if (copyToClipboard)
        ImGui::LogFinish();

    if (scrollToBottom)
        ImGui::SetScrollHereY(1.0f);

    scrollToBottom = false;
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaimFocus = false;
    int flags = ImGuiInputTextFlags_EnterReturnsTrue
            | ImGuiInputTextFlags_CallbackCompletion
            | ImGuiInputTextFlags_CallbackHistory;

    auto callback = [](ImGuiInputTextCallbackData *data) {
        auto console = (Console *) data->UserData;
        return console->TextEditCallback(data);
    };

    if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), flags, callback, (void *) this)) {
        auto s = str::trimmed(inputBuf);
        if (s[0])
            Execute(s.c_str());
        std::fill(std::begin(inputBuf), std::end(inputBuf), 0);
        reclaimFocus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaimFocus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

void Console::Execute(const char *commandLine)
{
    AddLog("# %s\n", commandLine);

    std::string commandString(commandLine);

    // Insert into history. First find match and delete it so it can be pushed to the back.
    historyPos = -1;
    for (size_t i = history.size() - 1; i >= 0; i--)
        if (history[i] == commandString) {
            history.erase(history.begin() + i);
            break;
        }

    history.push_back(commandString);

    scrollToBottom = true;
}

int Console::TextEditCallback(ImGuiInputTextCallbackData *data)
{
    switch (data->EventFlag) {
        case ImGuiInputTextFlags_CallbackCompletion: break;
        case ImGuiInputTextFlags_CallbackHistory: {
            const int prevHistoryPos = historyPos;
            if (data->EventKey == ImGuiKey_UpArrow) {
                if (historyPos == -1)
                    historyPos = history.size() - 1;
                else if (historyPos > 0)
                    historyPos--;
            } else if (data->EventKey == ImGuiKey_DownArrow) {
                if (historyPos != -1)
                    if (++historyPos >= history.size())
                        historyPos = -1;
            }

            if (prevHistoryPos != historyPos) {
                const char *historyStr = (historyPos >= 0) ? history[historyPos].c_str() : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, historyStr);
            }
        }
        default: break;
    }
    return 0;
}

void showConsole(bool *pOpen)
{
    static Console console;
    console.Draw("Example: Console", pOpen);
}
