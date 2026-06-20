#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "File.h"

#include <filesystem>
#include <string>
#include <vector>

class Directory {
public:
    Directory() = default;
    explicit Directory(std::filesystem::path path);

    const std::filesystem::path& path() const;
    std::string name() const;
    std::vector<File> files() const;
    std::vector<Directory> directories() const;

private:
    std::filesystem::path path_;
};

#endif
