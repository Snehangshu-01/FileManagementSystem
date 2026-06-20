#include "FileSystem.h"
#include "Utility.h"

#include <algorithm>
#include <system_error>

#ifdef _WIN32
#include <windows.h>

static void win32Search(const std::wstring& root, const std::wstring& query,
                        std::vector<SearchResult>& results, std::size_t maxResults) {
    if (results.size() >= maxResults) return;

    std::wstring pattern = root + L"\\*";
    WIN32_FIND_DATAW data;
    HANDLE hFind = FindFirstFileW(pattern.c_str(), &data);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        std::wstring name = data.cFileName;
        if (name == L"." || name == L"..") continue;

        std::wstring fullPath = root + L"\\" + name;
        bool isDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

        std::wstring nameLower = name;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::towlower);
        if (nameLower.find(query) != std::wstring::npos) {
            results.push_back({std::filesystem::path(fullPath), isDir});
        }

        if (isDir && !(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
            win32Search(fullPath, query, results, maxResults);
        }

    } while (results.size() < maxResults && FindNextFileW(hFind, &data));

    FindClose(hFind);
}
#endif

FileSystem::FileSystem() : currentPath_(std::filesystem::current_path()) {}

const std::filesystem::path& FileSystem::currentPath() const {
    return currentPath_;
}

bool FileSystem::changeDirectory(const std::string& target) {
    std::error_code error;
    const auto nextPath = resolvePath(target);

    if (!std::filesystem::exists(nextPath, error) || !std::filesystem::is_directory(nextPath, error)) {
        return false;
    }

    currentPath_ = std::filesystem::weakly_canonical(nextPath, error);
    if (error) {
        currentPath_ = nextPath;
    }

    return true;
}

Directory FileSystem::currentDirectory() const {
    return Directory(currentPath_);
}

std::vector<SearchResult> FileSystem::search(const std::string& query, std::size_t maxResults) const {
    std::vector<SearchResult> results;
    if (query.empty()) return results;

#ifdef _WIN32
    std::wstring wQuery(query.begin(), query.end());
    std::transform(wQuery.begin(), wQuery.end(), wQuery.begin(), ::towlower);
    win32Search(currentPath_.wstring(), wQuery, results, maxResults);
#else
    const auto normalizedQuery = Utility::toLower(query);
    std::error_code error;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(
             currentPath_, std::filesystem::directory_options::skip_permission_denied, error)) {
        if (results.size() >= maxResults) break;
        const auto name = Utility::toLower(entry.path().filename().string());
        if (name.find(normalizedQuery) != std::string::npos)
            results.push_back({entry.path(), entry.is_directory(error)});
    }
#endif
    return results;
}

File FileSystem::fileInfo(const std::filesystem::path& path) const {
    return File(std::filesystem::directory_entry(path));
}

std::filesystem::path FileSystem::resolvePath(const std::string& target) const {
    if (target == "..") {
        return currentPath_.parent_path();
    }

    std::filesystem::path path(target);
    if (path.is_absolute()) {
        return path;
    }

    return currentPath_ / path;
}