#pragma once

#include "common.h"

struct VuFS {
    std::unordered_map<std::string, std::vector<std::string>> m_MountPoints;
    void Mount(const std::string &virtualPath, const std::string &physicalPath);
    void Unmount(const std::string &path);
};

struct VuFile {
    std::string filePath{""};
    std::string fileName{""};
    std::string ext{""};
    bool is_dir = false;

    explicit VuFile(const tinydir_file &file);
};

struct VuDir {
    std::unique_ptr<tinydir_dir> dir;

    VuDir() : dir(std::make_unique<tinydir_dir>()) {}

    bool TryOpen(const std::string &path);

    std::string BaseName() const;

    void Fold(std::vector<VuFile> &file_list, void (*map_fn)(std::vector<VuFile> &, const tinydir_file &));
};

size_t GetFileSize(const std::string &path);
std::string GetBaseDir(const std::string &filepath);
