#include "Directory.h"

#include <system_error>

Directory::Directory(std::filesystem::path path) : path_(std::move(path)) {}

const std::filesystem::path& Directory::path() const {
    return path_;
}

std::string Directory::name() const {
    if (path_.has_filename()) {
        return path_.filename().string();
    }

    return path_.string();
}

std::vector<File> Directory::files() const {
    std::vector<File> result;
    std::error_code error;

    for (const auto& entry : std::filesystem::directory_iterator(path_, std::filesystem::directory_options::skip_permission_denied, error)) {
        if (entry.is_regular_file(error)) {
            result.emplace_back(entry);
        }
    }

    return result;
}

std::vector<Directory> Directory::directories() const {
    std::vector<Directory> result;
    std::error_code error;

    for (const auto& entry : std::filesystem::directory_iterator(path_, std::filesystem::directory_options::skip_permission_denied, error)) {
        if (entry.is_directory(error)) {
            result.emplace_back(entry.path());
        }
    }

    return result;
}
