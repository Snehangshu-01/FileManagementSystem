#ifndef STORAGE_H
#define STORAGE_H

#include <filesystem>

class Storage {
public:
    explicit Storage(std::filesystem::path databasePath = "filesystem.dat");

    void saveLastPath(const std::filesystem::path& path) const;
    std::filesystem::path loadLastPath() const;

private:
    std::filesystem::path databasePath_;
};

#endif
