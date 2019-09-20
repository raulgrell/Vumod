#include "FileUtils.h"
#include <fstream>

namespace file
{
    bool exists(const std::string &path)
    {
        if (FILE *file = fopen(path.c_str(), "r")) {
            fclose(file);
            return true;
        }
        return false;
    }

    size_t size(const std::string &path)
    {
        std::ifstream in(path, std::ifstream::binary | std::ifstream::ate);
        if (in.good()) {
            return in.tellg();
        } else {
            return 0;
        }
    }

    std::string baseDir(const std::string &filepath)
    {
        size_t index = filepath.find_last_of("/\\");
        if (index != std::string::npos)
            return filepath.substr(0, index);
        return std::string(".\\");
    }

    uint8_t *read(const std::string &path)
    {
        std::ifstream ifs(path, std::ios::binary | std::ios::ate);
        size_t pos = ifs.tellg();
        auto *result = new uint8_t[pos];
        ifs.seekg(0, std::ios::beg);
        ifs.read(reinterpret_cast<char *>(result), pos);
        return result;
    }

    size_t read(const std::string &path, std::vector<unsigned char> &buffer)
    {
        std::ifstream ifs(path, std::ios::binary | std::ios::ate);
        size_t pos = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        buffer.resize(pos);
        ifs.read(reinterpret_cast<char *>(&buffer[0]), pos);
        return pos;
    }

    size_t readText(const std::string &filePath,
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

    std::string readText(const std::string &path)
    {
        std::vector<unsigned char> fileContents = {};
        readText(path, fileContents);
        return std::string(fileContents.begin(), fileContents.end());
    }

    bool write(const std::string &path, uint8_t *buffer)
    {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        file.write(reinterpret_cast<char *>(buffer), sizeof(buffer));
        return true;
    }

    bool writeText(const std::string &path, const std::string &text)
    {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        file.write(text.c_str(), text.length());
        return true;
    }
}
