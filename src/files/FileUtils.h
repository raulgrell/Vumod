#pragma once

#include <string>
#include <vector>

namespace file
{
    bool exists(const std::string &path);
    size_t size(const std::string &path);

    uint8_t *read(const std::string &path);
    size_t read(const std::string &path, std::vector<unsigned char> &buffer);

    std::string readText(const std::string &path);
    size_t readText(const std::string &filePath, std::vector<unsigned char> &buffer);

    bool write(const std::string &path, uint8_t *buffer);
    bool writeText(const std::string &path, const std::string &text);

    std::string baseDir(const std::string &filepath);
}

