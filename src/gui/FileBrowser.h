#pragma once

#include <imgui.h>

#include <array>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

using ImGuiFileBrowserFlags = int;

enum ImGuiFileBrowserFlags_
{
    ImGuiFileBrowserFlags_SelectDirectory = 1 << 0, // select directory instead of regular file
    ImGuiFileBrowserFlags_EnterNewFilename = 1 << 1, // allow user to enter new filename when selecting regular file
    ImGuiFileBrowserFlags_NoModal = 1 << 2, // file browsing window is modal by default. Use a popup
    ImGuiFileBrowserFlags_NoTitleBar = 1 << 3, // hide window title bar
    ImGuiFileBrowserFlags_NoStatusBar = 1 << 4, // hide status bar at the bottom of browsing window
    ImGuiFileBrowserFlags_CloseOnEsc = 1 << 5, // close file browser when pressing 'ESC'
    ImGuiFileBrowserFlags_CreateNewDir = 1 << 6, // allow user to create new directory
};

namespace ImGui
{
    class FileBrowser
    {
        static constexpr size_t INPUT_NAME_BUF_SIZE = 512;

    public:
        explicit FileBrowser(ImGuiFileBrowserFlags flags = 0);
        FileBrowser(const FileBrowser &copyFrom);

        FileBrowser &operator=(const FileBrowser &copyFrom);

        void SetTitle(std::string title);
        void Open();
        void Close();
        void Display();

        bool IsOpen() const noexcept;
        bool HasSelected() const noexcept;
        std::filesystem::path GetSelected() const;
        void ClearSelected();

        bool SetPwd(const std::filesystem::path &pwd = std::filesystem::current_path());

    private:

        class ScopeGuard
        {
            std::function<void()> func;

        public:
            template<typename T>
            explicit ScopeGuard(T func) : func(std::move(func)) {}
            ~ScopeGuard() { func(); }
        };

        void SetPwdUncatched(const std::filesystem::path &pwd);

        ImGuiFileBrowserFlags flags;

        std::string title;
        std::string openLabel;

        bool openFlag;
        bool closeFlag;
        bool isOpened;
        bool ok;

        std::string statusStr;

        std::filesystem::path pwd;
        std::string selectedFilename;

        struct FileRecord
        {
            bool isDir {};
            std::string name;
            std::string showName;
        };

        std::vector<FileRecord> fileRecords;

        std::unique_ptr<std::array<char, INPUT_NAME_BUF_SIZE>> inputNameBuf;

        std::string openNewDirLabel;
        std::unique_ptr<std::array<char, INPUT_NAME_BUF_SIZE>> newDirNameBuf;
    };
}
