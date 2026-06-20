#ifndef FILE_H
#define FILE_H

#include <cstdint>
#include <filesystem>
#include <string>

class File {
public:
    File() = default;
    explicit File(const std::filesystem::directory_entry& entry);

    const std::filesystem::path& path() const;
    std::string name() const;
    std::uintmax_t size() const;
    bool isHidden() const;
    std::string lastWriteTime() const;

private:
    std::filesystem::path path_;
    std::uintmax_t size_ = 0;
    bool hidden_ = false;
    std::string lastWriteTime_;
};

#endif
