#include "common.h"

static size_t GetFileSize(const std::string &path)
{
    std::ifstream in(path, std::ifstream::binary | std::ifstream::ate);
    return in.tellg();
}

static std::string GetBaseDir(const std::string &filepath)
{
    int index = filepath.find_last_of("/\\");
    if (index != std::string::npos)
        return filepath.substr(0, index);
    return std::string("");
}

static bool FileExists(const std::string &path)
{
    if (FILE *file = fopen(path.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t *ReadFile(const std::string &path)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    size_t pos = ifs.tellg();
    uint8_t *result = new uint8_t[pos];
    ifs.seekg(0, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(result), pos);
    return result;
}

size_t ReadFile(const std::string &path, std::vector<unsigned char>& buffer)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    size_t pos = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    buffer.resize(pos);
    ifs.read(reinterpret_cast<char *>(&buffer[0]), pos);
    return pos;
}

size_t ReadTextFile(const std::string& filePath, std::vector<unsigned char>& buffer)
{
    std::ifstream file(filePath);
    if (file.fail()) {
        perror(filePath.c_str());
        return 0;
    }

    size_t fileSize = 0;
    if (file.seekg(0, std::ios::end).good())
        fileSize = (size_t)file.tellg();
    if (file.seekg(0, std::ios::beg).good())
        fileSize -= (size_t)file.tellg();

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

void Mount(VuFS &vf, const std::string &virtualPath, const std::string &physicalPath)
{
    vf.m_MountPoints[virtualPath].push_back(physicalPath);
}

void Unmount(VuFS &vf, const std::string &path)
{
    vf.m_MountPoints[path].clear();
}

bool ResolvePhysicalPath(VuFS &vf, const std::string &path, std::string &outPhysicalPath)
{
    if (path[0] != '/')
    {
        outPhysicalPath = path;
        return FileExists(path);
    }

    std::vector<std::string> dirs = split(path, '/');
    const std::string &virtualDir = dirs.front();

    if (vf.m_MountPoints.find(virtualDir) == vf.m_MountPoints.end() || vf.m_MountPoints[virtualDir].empty())
        return false;

    std::string remainder = path.substr(virtualDir.size() + 1, path.size() - virtualDir.size());
    for (const std::string &physicalPath : vf.m_MountPoints[virtualDir])
    {
        std::string path = physicalPath + remainder;
        if (FileExists(path))
        {
            outPhysicalPath = path;
            return true;
        }
    }
    return false;
}

uint8_t *ReadFile(VuFS &vf, const std::string &path)
{
    std::string physicalPath;
    return ResolvePhysicalPath(vf, path, physicalPath) ? ReadFile(physicalPath) : nullptr;
}

size_t ReadFile(VuFS &vf, const std::string &path, std::vector<unsigned char>& buffer)
{
    std::string physicalPath;
    return ResolvePhysicalPath(vf, path, physicalPath) ? ReadFile(physicalPath, buffer) : 0;
}

std::string ReadTextFile(VuFS &vf, const std::string &path)
{
    std::string physicalPath;
    return ResolvePhysicalPath(vf, path, physicalPath) ? ReadTextFile(physicalPath) : nullptr;
}

size_t ReadTextFile(VuFS &vf, const std::string &path, std::vector<unsigned char>& buffer)
{
    std::string physicalPath;
    return ResolvePhysicalPath(vf, path, physicalPath) ? ReadTextFile(physicalPath, buffer) : 0;
}

bool WriteFile(VuFS &vf, const std::string &path, uint8_t *buffer)
{
    std::string physicalPath;
    return ResolvePhysicalPath(vf, path, physicalPath) ? WriteFile(physicalPath, buffer) : false;
}

bool WriteTextFile(VuFS &vf, const std::string &path, const std::string &text)
{
    std::string physicalPath;
    return ResolvePhysicalPath(vf, path, physicalPath) ? WriteTextFile(physicalPath, text) : false;
}