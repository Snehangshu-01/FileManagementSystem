#include "Utility.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Utility {

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

std::string formatFileSize(std::uintmax_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    double size = static_cast<double>(bytes);
    int unitIndex = 0;

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        ++unitIndex;
    }

    std::ostringstream output;
    output << std::fixed << std::setprecision(unitIndex == 0 ? 0 : 1) << size << ' ' << units[unitIndex];
    return output.str();
}

std::string formatFileTime(const std::filesystem::file_time_type& time) {
    if (time == std::filesystem::file_time_type{}) {
        return "unknown";
    }

    const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(time);
    const auto cTime = std::chrono::system_clock::to_time_t(systemTime);
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &cTime);
#else
    localtime_r(&cTime, &localTime);
#endif

    std::ostringstream output;
    output << std::put_time(&localTime, "%Y-%m-%d %H:%M");
    return output.str();
}

bool isHidden(const std::filesystem::path& path) {
#ifdef _WIN32
    const auto attributes = GetFileAttributesW(path.wstring().c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_HIDDEN);
#else
    const auto name = path.filename().string();
    return !name.empty() && name.front() == '.';
#endif
}

} // namespace Utility
