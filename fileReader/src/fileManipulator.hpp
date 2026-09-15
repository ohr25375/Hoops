#pragma once
#ifndef FILE_MANIPULATOR
#define FILE_MANIPULATOR

#include <filesystem>
#include <vector>
#include <string>

namespace fileManipulator {
    std::vector<std::filesystem::path> batchFilePaths(std::filesystem::path rootPath, std::string extension);
    void sortFilePaths(std::vector<std::filesystem::path>& paths);
} // fileManipulator

#endif