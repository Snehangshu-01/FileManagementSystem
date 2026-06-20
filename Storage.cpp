#include "Storage.h"

#include <fstream>
#include <string>

Storage::Storage(std::filesystem::path databasePath) : databasePath_(std::move(databasePath)) {}

void Storage::saveLastPath(const std::filesystem::path& path) const {
    std::ofstream output(databasePath_, std::ios::trunc);
    if (output) {
        output << path.string() << '\n';
    }
}

std::filesystem::path Storage::loadLastPath() const {
    std::ifstream input(databasePath_);
    std::string path;

    if (std::getline(input, path) && !path.empty()) {
        return path;
    }

    return {};
}
