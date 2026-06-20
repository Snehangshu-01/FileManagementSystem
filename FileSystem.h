#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "Directory.h"
#include "File.h"

#include <filesystem>
#include <string>
#include <vector>

struct SearchResult {
    std::filesystem::path path;
    bool isDirectory = false;
};

class FileSystem {
public:
    FileSystem();

    const std::filesystem::path& currentPath() const;
    bool changeDirectory(const std::string& target);
    Directory currentDirectory() const;
    std::vector<SearchResult> search(const std::string& query, std::size_t maxResults = 100) const;
    File fileInfo(const std::filesystem::path& path) const;

private:
    std::filesystem::path currentPath_;

    std::filesystem::path resolvePath(const std::string& target) const;
};

#endif
