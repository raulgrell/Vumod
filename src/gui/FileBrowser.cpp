#include "FileBrowser.h"

#ifndef IMGUI_VERSION
#   error "include imgui.h before this header"
#endif

ImGui::FileBrowser::FileBrowser(ImGuiFileBrowserFlags flags)
        : flags(flags),
          openFlag(false), closeFlag(false), isOpened(false), ok(false),
          inputNameBuf(std::make_unique<std::array<char, INPUT_NAME_BUF_SIZE>>())
{
    if (flags & ImGuiFileBrowserFlags_CreateNewDir) {
        newDirNameBuf = std::make_unique<std::array<char, INPUT_NAME_BUF_SIZE>>();
    }

    inputNameBuf->at(0) = '\0';
    SetTitle("file browser");
    SetPwd(std::filesystem::current_path());
}

ImGui::FileBrowser::FileBrowser(const FileBrowser &copyFrom)
        : FileBrowser()
{
    *this = copyFrom;
}

ImGui::FileBrowser &ImGui::FileBrowser::operator=(const FileBrowser &copyFrom)
{
    flags = copyFrom.flags;
    SetTitle(copyFrom.title);

    openFlag = copyFrom.openFlag;
    closeFlag = copyFrom.closeFlag;
    isOpened = copyFrom.isOpened;
    ok = copyFrom.ok;

    statusStr = "";
    pwd = copyFrom.pwd;
    selectedFilename = copyFrom.selectedFilename;

    fileRecords = copyFrom.fileRecords;

    *inputNameBuf = *copyFrom.inputNameBuf;

    if (flags & ImGuiFileBrowserFlags_CreateNewDir) {
        newDirNameBuf = std::make_unique<std::array<char, INPUT_NAME_BUF_SIZE>>();
        *newDirNameBuf = *copyFrom.newDirNameBuf;
    }

    return *this;
}

void ImGui::FileBrowser::SetTitle(std::string newTitle)
{
    title = std::move(newTitle);
    openLabel = title + "##filebrowser_" + std::to_string(reinterpret_cast<size_t>(this));
    openNewDirLabel = "new dir##new_dir_" + std::to_string(reinterpret_cast<size_t>(this));
}

void ImGui::FileBrowser::Open()
{
    ClearSelected();
    statusStr = std::string();
    openFlag = true;
    closeFlag = false;
}

void ImGui::FileBrowser::Close()
{
    ClearSelected();
    statusStr = std::string();
    closeFlag = true;
    openFlag = false;
}

bool ImGui::FileBrowser::IsOpen() const noexcept
{
    return isOpened;
}

void ImGui::FileBrowser::Display()
{
    PushID(this);
    ScopeGuard exitThis([this] {
        openFlag = false;
        closeFlag = false;
        PopID();
    });

    if (openFlag)
        OpenPopup(openLabel.c_str());
    isOpened = false;

    // open the popup window

    if (openFlag && (flags & ImGuiFileBrowserFlags_NoModal))
        SetNextWindowSize(ImVec2(700, 450));
    else
        SetNextWindowSize(ImVec2(700, 450), ImGuiCond_FirstUseEver);
    if (flags & ImGuiFileBrowserFlags_NoModal) {
        if (!BeginPopup(openLabel.c_str()))
            return;
    } else if (!BeginPopupModal(openLabel.c_str(), nullptr,
                                flags & ImGuiFileBrowserFlags_NoTitleBar ? ImGuiWindowFlags_NoTitleBar : 0)) {
        return;
    }
    isOpened = true;
    ScopeGuard endPopup([] { EndPopup(); });

    // display elements in pwd

    int secIdx = 0, newPwdLastSecIdx = -1;
    for (auto &sec : pwd) {
#ifdef _WIN32
        if (secIdx == 1) {
            ++secIdx;
            continue;
        }
#endif
        PushID(secIdx);
        if (secIdx > 0)
            SameLine();
        if (SmallButton(sec.u8string().c_str()))
            newPwdLastSecIdx = secIdx;
        PopID();
        ++secIdx;
    }

    if (newPwdLastSecIdx >= 0) {
        int i = 0;
        std::filesystem::path newPwd;
        for (auto &sec : pwd) {
            if (i++ > newPwdLastSecIdx)
                break;
            newPwd /= sec;
        }
#ifdef _WIN32
        if (newPwdLastSecIdx == 0)
            newPwd /= "\\";
#endif
        SetPwd(newPwd);
    }

    SameLine();

    if (SmallButton("*"))
        SetPwd(pwd);

    if (newDirNameBuf) {
        SameLine();
        if (SmallButton("+")) {
            OpenPopup(openNewDirLabel.c_str());
            (*newDirNameBuf)[0] = '\0';
        }

        if (BeginPopup(openNewDirLabel.c_str())) {
            ScopeGuard endNewDirPopup([] { EndPopup(); });

            InputText("name", newDirNameBuf->data(), newDirNameBuf->size());
            SameLine();
            if (Button("ok") && (*newDirNameBuf)[0] != '\0') {
                ScopeGuard closeNewDirPopup([] { CloseCurrentPopup(); });
                if (create_directory(pwd / newDirNameBuf->data()))
                    SetPwd(pwd);
                else
                    statusStr = "failed to create " + std::string(newDirNameBuf->data());
            }
        }
    }

    // browse files in a child window

    float reserveHeight = GetTextLineHeightWithSpacing();
    std::filesystem::path newPwd;
    bool setNewPwd = false;
    if (!(flags & ImGuiFileBrowserFlags_SelectDirectory) && (flags & ImGuiFileBrowserFlags_EnterNewFilename))
        reserveHeight += GetTextLineHeightWithSpacing();
    {
        BeginChild("ch", ImVec2(0, -reserveHeight), true,
                   (flags & ImGuiFileBrowserFlags_NoModal) ? ImGuiWindowFlags_AlwaysHorizontalScrollbar : 0);
        ScopeGuard endChild([] { EndChild(); });

        for (auto &rsc : fileRecords) {
            const bool selected = selectedFilename == rsc.name;
            if (Selectable(rsc.showName.c_str(), selected, ImGuiSelectableFlags_DontClosePopups)) {
                if (selected) {
                    selectedFilename = std::string();
                    (*inputNameBuf)[0] = '\0';
                } else if (rsc.name != "..") {
                    if ((rsc.isDir && (flags & ImGuiFileBrowserFlags_SelectDirectory)) ||
                        (!rsc.isDir && !(flags & ImGuiFileBrowserFlags_SelectDirectory))) {
                        selectedFilename = rsc.name;
                        if (!(flags & ImGuiFileBrowserFlags_SelectDirectory))
                            strcpy(inputNameBuf->data(), selectedFilename.c_str());
                    }
                }
            }

            if (IsItemClicked(0) && IsMouseDoubleClicked(0) && rsc.isDir) {
                setNewPwd = true;
                newPwd = (rsc.name != "..") ? (pwd / rsc.name) : pwd.parent_path();
            }
        }
    }

    if (setNewPwd)
        SetPwd(newPwd);

    if (!(flags & ImGuiFileBrowserFlags_SelectDirectory) && (flags & ImGuiFileBrowserFlags_EnterNewFilename)) {
        PushID(this);
        ScopeGuard popTextID([] { PopID(); });

        PushItemWidth(-1);
        if (InputText("", inputNameBuf->data(), inputNameBuf->size()))
            selectedFilename = inputNameBuf->data();
        PopItemWidth();
    }

    if (!(flags & ImGuiFileBrowserFlags_SelectDirectory)) {
        if (Button(" ok ") && !selectedFilename.empty()) {
            ok = true;
            CloseCurrentPopup();
        }
    } else {
        if (selectedFilename.empty()) {
            if (Button(" ok ")) {
                ok = true;
                CloseCurrentPopup();
            }
        } else if (Button("open"))
            SetPwd(pwd / selectedFilename);
    }

    SameLine();

    int escIdx = GetIO().KeyMap[ImGuiKey_Escape];
    if (Button("cancel") || closeFlag ||
        ((flags & ImGuiFileBrowserFlags_CloseOnEsc) && IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
         escIdx >= 0 && IsKeyPressed(escIdx)))
        CloseCurrentPopup();

    if (!statusStr.empty() && !(flags & ImGuiFileBrowserFlags_NoStatusBar)) {
        SameLine();
        Text("%s", statusStr.c_str());
    }
}

bool ImGui::FileBrowser::HasSelected() const noexcept
{
    return ok;
}

bool ImGui::FileBrowser::SetPwd(const std::filesystem::path &dir)
{
    try {
        SetPwdUncatched(dir);
        return true;
    }
    catch (const std::exception &err) {
        statusStr = std::string("last error: ") + err.what();
    }
    catch (...) {
        statusStr = "last error: unknown";
    }

    SetPwdUncatched(std::filesystem::current_path());
    return false;
}

std::filesystem::path ImGui::FileBrowser::GetSelected() const
{
    return pwd / selectedFilename;
}

void ImGui::FileBrowser::ClearSelected()
{
    selectedFilename = std::string();
    (*inputNameBuf)[0] = '\0';
    ok = false;
}

void ImGui::FileBrowser::SetPwdUncatched(const std::filesystem::path &dir)
{
    fileRecords = {FileRecord {true, "..", "[D] .."}};

    for (auto &p : std::filesystem::directory_iterator(dir)) {
        FileRecord rcd;

        if (p.is_regular_file())
            rcd.isDir = false;
        else if (p.is_directory())
            rcd.isDir = true;
        else
            continue;

        rcd.name = p.path().filename().string();
        if (rcd.name.empty())
            continue;

        rcd.showName = (rcd.isDir ? "[D] " : "[F] ") + p.path().filename().u8string();
        fileRecords.push_back(rcd);
    }

    std::sort(fileRecords.begin(), fileRecords.end(), [](const FileRecord &L, const FileRecord &R) {
        return (L.isDir ^ R.isDir) ? L.isDir : (L.name < R.name);
    });

    pwd = absolute(dir);
    selectedFilename = std::string();
    (*inputNameBuf)[0] = '\0';
}
