#pragma once

#include <string>
#include <vector>

bool FileExists(const std::string &path);
uint8_t *ReadFile(const std::string &path);
size_t ReadFile(const std::string &path, std::vector<unsigned char> &buffer);
std::string ReadTextFile(const std::string &path);
bool WriteFile(const std::string &path, uint8_t *buffer);
bool WriteTextFile(const std::string &path, const std::string &text);
size_t GetFileSize(const std::string &path);
std::string GetBaseDir(const std::string &filepath);

