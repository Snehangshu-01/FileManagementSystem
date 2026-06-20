#ifndef UTILITY_H
#define UTILITY_H

#include <chrono>
#include <filesystem>
#include <string>

namespace Utility {
std::string toLower(std::string value);
std::string formatFileSize(std::uintmax_t bytes);
std::string formatFileTime(const std::filesystem::file_time_type& time);
bool isHidden(const std::filesystem::path& path);
}

#endif
