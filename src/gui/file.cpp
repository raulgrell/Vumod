
#include "../common.h"

#define MAX_FILE_DIALOG_NAME_BUFFER 1024

class FileGui {
private:
    std::vector<VuFile> m_FileList;
    std::string m_SelectedFileName;
    std::string m_CurrentPath;
    std::vector<std::string> m_CurrentPath_Decomposition;
    std::string m_CurrentFilterExt;

public:
    bool Valid = false;

public:
    static char FileNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER];
    static int FilterIndex;

    static FileGui *Instance() {
        static auto *_instance = new FileGui();
        return _instance;
    }

public:
    bool FileDialog(const char *vName, const char *vFilters = 0, std::string vPath = ".",
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

inline bool ReplaceString(std::string &str, const std::string &oldStr, const std::string &newStr) {
    bool found = false;
    size_t pos = 0;
    while ((pos = str.find(oldStr, pos)) != std::string::npos) {
        found = true;
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
    return found;
}

inline void AppendToBuffer(char *vBuffer, int vBufferLen, const std::string &vStr) {
    std::string st = vStr;
    if (!st.empty() && st != "\n")
        ReplaceString(st, "\n", "");
    int slen = strlen(vBuffer);
    vBuffer[slen] = '\0';
    std::string str = std::string(vBuffer);
    if (!str.empty()) str += "\n";
    str += vStr;
    int len = vBufferLen - 1;
    if (len > str.size()) len = str.size();
#ifdef MINGW32
    strncpy_s(vBuffer, vBufferLen, str.c_str(), len);
#else
    strncpy(vBuffer, str.c_str(), len);
#endif
    vBuffer[len] = '\0';
}

inline void ResetBuffer(char *vBuffer) {
    vBuffer[0] = '\0';
}

char FileGui::FileNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";
int FileGui::FilterIndex = 0;

static bool file_comparator(const VuFile &a, const VuFile &b) {
    return a.is_dir != b.is_dir ? a.is_dir : a.fileName < b.fileName;
}

void FileGui::ScanDir(const std::string &vPath) {
    VuDir vd;
    if (!vd.TryOpen(vPath) || !vd.TryOpen(".")) {
        perror("Error getting file");
        return;
    }

    SetCurrentDir(vPath);
    m_FileList.clear();
    vd.Fold(m_FileList, [](auto &file_list, const auto &file) { file_list.emplace_back(file); });
    std::sort(m_FileList.begin(), m_FileList.end(), file_comparator);
}

void FileGui::SetCurrentDir(const std::string &vPath) {
    m_CurrentPath = std::string(vPath);
    ReplaceString(m_CurrentPath, "/*", "");
    m_CurrentPath_Decomposition = splitString(m_CurrentPath, '/');
}

void FileGui::ComposeNewPath(std::vector<std::string>::iterator vIter) {
    m_CurrentPath = "";
    while (vIter != m_CurrentPath_Decomposition.begin()) {
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
        if (!vDefaultFileName.empty()) {
            ResetBuffer(FileNameBuffer);
            AppendToBuffer(FileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, vDefaultFileName);
        }
        ScanDir(vPath);
    }

    // show current path
    bool pathClick = false;
    for (auto itPathDecomp = m_CurrentPath_Decomposition.begin();
         itPathDecomp != m_CurrentPath_Decomposition.end(); ++itPathDecomp) {
        if (itPathDecomp != m_CurrentPath_Decomposition.begin())
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
                        if (m_CurrentPath_Decomposition.size() > 1) {
                            auto itPathDecomp = m_CurrentPath_Decomposition.end() - 2;
                            ComposeNewPath(itPathDecomp);
                        }
                    } else {
                        m_CurrentPath += "/" + infos.fileName;
                    }
                    pathClick = true;
                } else {
                    m_SelectedFileName = infos.fileName;
                    ResetBuffer(FileNameBuffer);
                    AppendToBuffer(FileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, m_SelectedFileName);
                }
                break;
            }
        }
    }

    if (pathClick) {
        ScanDir(m_CurrentPath);
        m_CurrentPath_Decomposition = splitString(m_CurrentPath, '/');
        if (m_CurrentPath_Decomposition.size() == 2) {
            if (m_CurrentPath_Decomposition[1].empty())
                m_CurrentPath_Decomposition.erase(m_CurrentPath_Decomposition.end() - 1);
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
    return std::string(FileNameBuffer);
}

std::string FileGui::GetCurrentFilter() const {
    return m_CurrentFilterExt;
}
