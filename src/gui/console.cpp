#include "common.h"

struct ConsoleApp {
    char InputBuf[256]{0};
    std::vector<std::string> Items;
    std::vector<std::string> History;
    int HistoryPos = -1; // -1: new line, 0..History.Size-1 browsing history.
    bool AutoScroll = true;
    bool ScrollToBottom = true;

    ConsoleApp() {
        ClearLog();
        AddLog("Welcome to Dear ImGui!");
    }

    ~ConsoleApp() {
        ClearLog();
    }

    void ClearLog() {
        Items.clear();
        ScrollToBottom = true;
    }

    void AddLog(const char *fmt, ...) IM_FMTARGS(2) {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        Items.emplace_back(buf);
        if (AutoScroll)
            ScrollToBottom = true;
    }

    void Draw(const char *title, bool *p_open) {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Close Console"))
                *p_open = false;
            ImGui::EndPopup();
        }

        ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

        if (ImGui::SmallButton("Add Dummy Text"))
            AddLog("text %llu", Items.size());
        ImGui::SameLine();
        if (ImGui::SmallButton("Add Dummy Error"))
            AddLog("[error] something went wrong");
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear"))
            ClearLog();
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");
        ImGui::SameLine();
        if (ImGui::SmallButton("Scroll to bottom"))
            ScrollToBottom = true;

        ImGui::Separator();

        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y +
                                               ImGui::GetFrameHeightWithSpacing();             // 1 separator, 1 input text
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false,
                          ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::Selectable("Clear"))
                ClearLog();
            ImGui::EndPopup();
        }

        // TODO: display items starting from the bottom
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        if (copy_to_clipboard)
            ImGui::LogToClipboard();

        for (int i = 0; i < Items.size(); i++) {
            const char *item = Items[i].c_str();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
            ImGui::TextUnformatted(item);
            ImGui::PopStyleColor();
        }

        if (copy_to_clipboard)
            ImGui::LogFinish();
        if (ScrollToBottom)
            ImGui::SetScrollHereY(1.0f);

        ScrollToBottom = false;
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        int flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
                    ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), flags, &TextEditCallbackStub, (void *) this)) {
            auto s = std::string(InputBuf);
            trim(s);
            if (s[0])
                ExecCommand(s.c_str());
            std::fill(std::begin(InputBuf), std::end(InputBuf), 0);
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void ExecCommand(const char *command_line) {
        AddLog("# %s\n", command_line);

        std::string command_string(command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.size() - 1; i >= 0; i--)
            if (History[i] == command_string) {
                History.erase(History.begin() + i);
                break;
            }

        History.push_back(command_string);
        AddLog("Unknown command: '%s'", command_line);
        ScrollToBottom = true;
    }

    static int TextEditCallbackStub(
            ImGuiInputTextCallbackData *data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
    {
        auto console = (ConsoleApp *) data->UserData;
        return console->TextEditCallback(data);
    }

    int TextEditCallback(ImGuiInputTextCallbackData *data) {
        switch (data->EventFlag) {
            case ImGuiInputTextFlags_CallbackCompletion: {
                break;
            }
            case ImGuiInputTextFlags_CallbackHistory: {
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow) {
                    if (HistoryPos == -1)
                        HistoryPos = History.size() - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                } else if (data->EventKey == ImGuiKey_DownArrow) {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.size())
                            HistoryPos = -1;
                }

                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos) {
                    const char *history_str = (HistoryPos >= 0) ? History[HistoryPos].c_str() : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
            default:
                break;
        }
        return 0;
    }
};

static void ShowConsole(bool *p_open) {
    static ConsoleApp console;
    console.Draw("Example: Console", p_open);
}