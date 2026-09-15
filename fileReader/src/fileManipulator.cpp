#include "fileManipulator.hpp"

#include <algorithm>

namespace fileManipulator {
    std::vector<std::filesystem::path> batchFilePaths(std::filesystem::path rootPath, std::string extension) {
        std::vector<std::filesystem::path> files;
        for (auto &p : std::filesystem::recursive_directory_iterator(rootPath)) {
            if (p.path().extension() != extension) continue;
            files.push_back(p);
        }
        return files;
    }

    void sortFilePaths(std::vector<std::filesystem::path> &paths) {
        std::sort(paths.begin(), paths.end(), [](std::filesystem::path const l, std::filesystem::path const r){
            return l.filename().string().size() < r.filename().string().size()
                || l.filename() < r.filename();
        });
    }
}