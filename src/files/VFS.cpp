#include "VFS.h"

#include <fstream>
#include <files/FileUtils.h>
#include <util/StringUtils.h>

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

bool VFS::TryResolvePath(const std::string &path, std::string &outPhysicalPath)
{
    if (path[0] != '/') {
        outPhysicalPath = path;
        return file::exists(path);
    }

    std::vector<std::string> dirs = str::split(path, '/');
    const std::string &vDir = dirs.front();

    if (m_MountPoints.find(vDir) == m_MountPoints.end() ||
        m_MountPoints[vDir].empty())
        return false;

    std::string rest = path.substr(vDir.size() + 1, path.size() - vDir.size());
    for (const std::string &physicalPath : m_MountPoints[vDir]) {
        std::string full_path = physicalPath + rest;
        if (file::exists(full_path)) {
            outPhysicalPath = full_path;
            return true;
        }
    }
    return false;
}

uint8_t *VFS::ReadFile(const std::string &path)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? ReadFile(physicalPath) : nullptr;
}

size_t VFS::ReadFile(const std::string &path,
                     std::vector<unsigned char> &buffer)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? ReadFile(physicalPath, buffer)
                                              : 0;
}

std::string VFS::ReadTextFile(const std::string &path)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? ReadTextFile(physicalPath)
                                              : nullptr;
}

size_t VFS::ReadTextFile(const std::string &path,
                         std::vector<unsigned char> &buffer)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? ReadTextFile(physicalPath, buffer)
                                              : 0;
}

bool VFS::WriteFile(const std::string &path, uint8_t *buffer)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? WriteFile(physicalPath, buffer)
                                              : false;
}

bool VFS::WriteTextFile(const std::string &path, const std::string &text)
{
    std::string physicalPath;
    return TryResolvePath(path, physicalPath) ? WriteTextFile(physicalPath, text)
                                              : false;
}

VuFile::VuFile(const tinydir_file &file)
{
    filePath = file.path;
    fileName = file.name;
    ext = file.extension;
    is_dir = file.is_dir;
}

void VFS::Mount(const std::string &virtualPath,
                const std::string &physicalPath)
{
    m_MountPoints[virtualPath].push_back(physicalPath);
}

void VFS::Unmount(const std::string &path)
{
    m_MountPoints[path].clear();
}
