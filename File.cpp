#include "File.h"

#include "Utility.h"

#include <system_error>

File::File(const std::filesystem::directory_entry& entry) : path_(entry.path()) {
    std::error_code error;
    if (entry.is_regular_file(error)) {
        size_ = entry.file_size(error);
        if (error) {
            size_ = 0;
        }
    }

    hidden_ = Utility::isHidden(path_);
    lastWriteTime_ = Utility::formatFileTime(entry.last_write_time(error));
}

const std::filesystem::path& File::path() const {
    return path_;
}

std::string File::name() const {
    return path_.filename().string();
}

std::uintmax_t File::size() const {
    return size_;
}

bool File::isHidden() const {
    return hidden_;
}

std::string File::lastWriteTime() const {
    return lastWriteTime_;
}
