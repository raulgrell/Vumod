#pragma once

#include "common.h"

#include <tinydir/tinydir.h>

#include <memory>
#include <unordered_map>
#include <vector>

struct VFS
{
    std::unordered_map<std::string, std::vector<std::string>> m_MountPoints;

    void Mount(const std::string &virtualPath, const std::string &physicalPath);
    void Unmount(const std::string &path);

    bool TryResolvePath(const std::string &path, std::string &outPhysicalPath);

    uint8_t *ReadFile(const std::string &path);
    size_t ReadFile(const std::string &path, std::vector<unsigned char> &buffer);

    std::string ReadTextFile(const std::string &path);
    size_t ReadTextFile(const std::string &path, std::vector<unsigned char> &buffer);

    bool WriteFile(const std::string &path, uint8_t *buffer);
    bool WriteTextFile(const std::string &path, const std::string &text);
};

struct VuFile
{
    std::string filePath {""};
    std::string fileName {""};
    std::string ext {""};
    bool is_dir = false;

    explicit VuFile(const tinydir_file &file);
};

struct VuDir
{
    std::unique_ptr<tinydir_dir> dir;

    VuDir() : dir(std::make_unique<tinydir_dir>()) {}

    std::string BaseName() const;

    bool TryOpen(const std::string &path);

    void Fold(std::vector<VuFile> &file_list,
              void (*map_fn)(std::vector<VuFile> &, const tinydir_file &));
};

