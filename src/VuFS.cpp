#include "VuFS.h"

#include <fstream>

size_t GetFileSize(const std::string &path)
{
    std::ifstream in(path, std::ifstream::binary | std::ifstream::ate);
    if (in.good()) {
        return in.tellg();
    } else {
        return 0;
    }
}

std::string GetBaseDir(const std::string &filepath)
{
    size_t index = filepath.find_last_of("/\\");
    if (index != std::string::npos)
        return filepath.substr(0, index);
    return std::string(".\\");
}

bool FileExists(const std::string &path)
{
    if (FILE *file = fopen(path.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

uint8_t *ReadFile(const std::string &path)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    size_t pos = ifs.tellg();
    auto *result = new uint8_t[pos];
    ifs.seekg(0, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(result), pos);
    return result;
}

size_t ReadFile(const std::string &path, std::vector<unsigned char> &buffer)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    size_t pos = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    buffer.resize(pos);
    ifs.read(reinterpret_cast<char *>(&buffer[0]), pos);
    return pos;
}

size_t ReadTextFile(const std::string &filePath,
                    std::vector<unsigned char> &buffer)
{
    std::ifstream file(filePath);
    if (file.fail()) {
        perror(filePath.c_str());
        return 0;
    }

    size_t fileSize = 0;
    if (file.seekg(0, std::ios::end).good())
        fileSize = (size_t) file.tellg();
    if (file.seekg(0, std::ios::beg).good())
        fileSize -= (size_t) file.tellg();

    if (fileSize > 0) {
        buffer.resize(fileSize);
        file.read(reinterpret_cast<char *>(&buffer[0]), fileSize);
    } else {
        buffer.clear();
    }

    return fileSize;
}

std::string ReadTextFile(const std::string &path)
{
    std::vector<unsigned char> fileContents = {};
    ReadTextFile(path, fileContents);
    return std::string(fileContents.begin(), fileContents.end());
}

bool WriteFile(const std::string &path, uint8_t *buffer)
{
    std::ofstream file(path, std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<char *>(buffer), sizeof(buffer));
    return true;
}

bool WriteTextFile(const std::string &path, const std::string &text)
{
    std::ofstream file(path, std::ios::out | std::ios::binary);
    file.write(text.c_str(), text.length());
    return true;
}

std::string VuDir::BaseName() const
{
    const std::string path {dir->path};
    auto lastSlash = path.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return path;
    }
    return path.substr(lastSlash + 1);
}

bool VuDir::TryOpen(const std::string &path)
{
    return tinydir_open(dir.get(), path.c_str()) != -1;
}

void VuDir::Fold(std::vector<VuFile> &file_list, void (*map_fn)(std::vector<VuFile> &, const tinydir_file &))
{
    while (dir->has_next) {
        tinydir_file file;
        if (tinydir_readfile(dir.get(), &file) == -1) {
            perror("Error getting file");
            return;
        }

        map_fn(file_list, file);

        if (tinydir_next(dir.get()) == -1) {
            perror("Error getting next file");
            return;
        }
    }
}

bool VuFS::TryResolvePath(const std::string &path,
                          std::string &outPhysicalPath)
{
    if (path[0] != '/') {
        outPhysicalPath = path;
        return FileExists(path);
    }

    std::vector<std::string> dirs = splitString(path, '/');
    const std::string &virtualDir = dirs.front();

    if (m_MountPoints.find(virtualDir) == m_MountPoints.end() ||
        m_MountPoints[virtualDir].empty())
        return false;

    std::string remainder =
            path.substr(virtualDir.size() + 1, path.size() - virtualDir.size());
    for (const std::string &physicalPath : m_MountPoints[virtualDir]) {
        std::string full_path = physicalPath + remainder;
        if (FileExists(full_path)) {
            outPhysicalPath = full_path;
            return true;
        }
    }
    return false;
}

uint8_t *VuFS::ReadFile(const std::string &path)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? ReadFile(physicalPath) : nullptr;
}

size_t VuFS::ReadFile(const std::string &path,
                      std::vector<unsigned char> &buffer)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? ReadFile(physicalPath, buffer)
                                              : 0;
}

std::string VuFS::ReadTextFile(const std::string &path)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? ReadTextFile(physicalPath)
                                              : nullptr;
}

size_t VuFS::ReadTextFile(const std::string &path,
                          std::vector<unsigned char> &buffer)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? ReadTextFile(physicalPath, buffer)
                                              : 0;
}

bool VuFS::WriteFile(const std::string &path, uint8_t *buffer)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? WriteFile(physicalPath, buffer)
                                              : false;
}

bool VuFS::WriteTextFile(const std::string &path, const std::string &text)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? WriteTextFile(physicalPath, text)
                                              : false;
}

VuFile::VuFile(const tinydir_file &file)
{
    fileName = file.name;
    filePath = file.path;
    ext = file.extension;
    is_dir = file.is_dir;
}

void VuFS::Mount(const std::string &virtualPath,
                 const std::string &physicalPath)
{
    m_MountPoints[virtualPath].push_back(physicalPath);
}

void VuFS::Unmount(const std::string &path) { m_MountPoints[path].clear(); }
