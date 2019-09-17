
#include <files/VFS.h>
#include <algorithm>
#include "../common.h"

#define MAX_FILE_DIALOG_NAME_BUFFER 1024

struct FileGui {
    std::vector<VuFile> m_FileList;
    std::string m_SelectedFileName;
    std::string m_CurrentPath;
    std::vector<std::string> m_SplitPath;
    std::string m_CurrentFilterExt;

    bool Valid = false;

    static char FileNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER];
    static int FilterIndex;

    static FileGui *Instance() {
        static auto *_instance = new FileGui();
        return _instance;
    }

    bool FileDialog(const char *vName, const char *vFilters = nullptr, std::string vPath = ".",
                    const std::string &vDefaultFileName = "");

    std::string GetFilepathName() const;

    std::string GetCurrentPath() const;

    std::string GetCurrentFileName() const;

    std::string GetCurrentFilter() const;

private:
    void ScanDir(const std::string &vPath);

    void SetCurrentDir(const std::string &vPath);

    void ComposeNewPath(std::vector<std::string>::iterator vIter);
};

char FileGui::FileNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";
int FileGui::FilterIndex = 0;

void FileGui::ScanDir(const std::string &vPath) {
    VuDir vd;
    if (!vd.TryOpen(vPath) || !vd.TryOpen(".")) {
        perror("Error getting file");
        return;
    }

    SetCurrentDir(vPath);
    m_FileList.clear();
    vd.Fold(m_FileList, [](auto &file_list, const auto &file) { file_list.emplace_back(file); });
    std::sort(m_FileList.begin(), m_FileList.end(), [](const VuFile &a, const VuFile &b) {
        return a.is_dir != b.is_dir ? a.is_dir : a.fileName < b.fileName;
    });
}

void FileGui::SetCurrentDir(const std::string &vPath) {
    m_CurrentPath = std::string(vPath);
    m_SplitPath = str::split(m_CurrentPath, '/');
}

void FileGui::ComposeNewPath(std::vector<std::string>::iterator vIter) {
    m_CurrentPath = "";
    while (vIter != m_SplitPath.begin()) {
        if (!m_CurrentPath.empty())
            m_CurrentPath = *vIter + "/" + m_CurrentPath;
        else
            m_CurrentPath = *vIter;
        vIter--;
    }

    if (!m_CurrentPath.empty())
        m_CurrentPath = *vIter + "/" + m_CurrentPath;
    else
        m_CurrentPath = *vIter + "/";
}

bool
FileGui::FileDialog(const char *vName, const char *vFilters, std::string vPath, const std::string &vDefaultFileName) {
    bool res = false;
    ImGui::Begin(vName);

    if (vPath.empty()) vPath = ".";

    if (m_FileList.empty()) {
        ScanDir(vPath);
    }

    // show current path
    bool pathClick = false;
    for (auto itPathDecomp = m_SplitPath.begin();
         itPathDecomp != m_SplitPath.end(); ++itPathDecomp) {
        if (itPathDecomp != m_SplitPath.begin())
            ImGui::SameLine();
        if (ImGui::Button((*itPathDecomp).c_str())) {
            ComposeNewPath(itPathDecomp);
            pathClick = true;
            break;
        }
    }

    ImVec2 size = ImGui::GetContentRegionMax() - ImVec2(0.0f, 120.0f);

    ImGui::BeginChild("##FileDialog_FileList", size);

    for (const auto &infos : m_FileList) {
        std::string str;
        str = (infos.is_dir) ? "[Dir] " + infos.fileName : "[File] " + infos.fileName;
        if (!m_CurrentFilterExt.empty() && infos.ext != m_CurrentFilterExt) {
            if (ImGui::Selectable(str.c_str(), (infos.fileName == m_SelectedFileName))) {
                if (infos.is_dir) {
                    if (infos.fileName == "..") {
                        if (m_SplitPath.size() > 1) {
                            auto itPathDecomp = m_SplitPath.end() - 2;
                            ComposeNewPath(itPathDecomp);
                        }
                    } else {
                        m_CurrentPath += "/" + infos.fileName;
                    }
                    pathClick = true;
                } else {
                    m_SelectedFileName = infos.fileName;
                }
                break;
            }
        }
    }

    if (pathClick) {
        ScanDir(m_CurrentPath);
        m_SplitPath = str::split(m_CurrentPath, '/');
        if (m_SplitPath.size() == 2) {
            if (m_SplitPath[1].empty())
                m_SplitPath.erase(m_SplitPath.end() - 1);
        }

    }

    ImGui::EndChild();
    ImGui::Text("File Name : ");
    ImGui::SameLine();

    float width = ImGui::GetContentRegionAvail().x;
    if (vFilters != nullptr)
        width -= 120.0f;
    ImGui::PushItemWidth(width);
    ImGui::InputText("##FileName", FileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER);
    ImGui::PopItemWidth();

    if (vFilters != nullptr) {
        ImGui::SameLine();

        ImGui::PushItemWidth(100.0f);
        bool comboClick = ImGui::Combo("##Filters", &FilterIndex, vFilters) || m_CurrentFilterExt.empty();
        ImGui::PopItemWidth();
        if (comboClick) {
            int itemIdx = 0;
            const char *p = vFilters;
            while (*p) {
                if (FilterIndex == itemIdx) {
                    m_CurrentFilterExt = std::string(p);
                    break;
                }
                p += strlen(p) + 1;
                itemIdx++;
            }
        }
    }

    if (ImGui::Button("Cancel")) {
        Valid = false;
        res = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Ok")) {
        Valid = true;
        res = true;
    }

    ImGui::End();

    if (res)
        m_FileList.clear();

    return res;
}

std::string FileGui::GetFilepathName() const {
    return GetCurrentPath() + "/" + GetCurrentFileName();
}

std::string FileGui::GetCurrentPath() const {
    return m_CurrentPath;
}

std::string FileGui::GetCurrentFileName() const {
    return m_SelectedFileName;
}

std::string FileGui::GetCurrentFilter() const {
    return m_CurrentFilterExt;
}
