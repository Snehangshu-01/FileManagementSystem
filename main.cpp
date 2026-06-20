#include "CommandParser.h"
#include "FileSystem.h"
#include "Storage.h"
#include "Utility.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace {

void printHelp() {
    std::cout << "Commands:\n"
              << "  ls | dir              List files and folders\n"
              << "  cd <path>             Change directory\n"
              << "  pwd                   Print current directory\n"
              << "  search <name>         Recursively search from current directory\n"
              << "  info <path>           Show file metadata\n"
              << "  help                  Show commands\n"
              << "  exit | quit           Close the program\n";
}

void printDirectory(const Directory& directory) {
    for (const auto& child : directory.directories()) {
        std::cout << "[D] " << child.name() << '\n';
    }

    for (const auto& file : directory.files()) {
        std::cout << "[F] " << file.name() << "  " << Utility::formatFileSize(file.size()) << '\n';
    }
}

std::string joinArgs(const std::vector<std::string>& args) {
    std::string result;
    for (const auto& arg : args) {
        if (!result.empty()) {
            result += ' ';
        }
        result += arg;
    }
    return result;
}

} // namespace

int main() {
    FileSystem fileSystem;
    CommandParser parser;
    Storage storage;

    const auto lastPath = storage.loadLastPath();
    if (!lastPath.empty()) {
        fileSystem.changeDirectory(lastPath.string());
    }

    std::cout << "FileManagementSystem bare bones\n";
    printHelp();

    std::string input;
    while (true) {
        std::cout << "\n" << fileSystem.currentPath().string() << "\n> ";
        if (!std::getline(std::cin, input)) {
            break;
        }

        const auto command = parser.parse(input);
        switch (command.type) {
        case CommandType::List:
            printDirectory(fileSystem.currentDirectory());
            break;
        case CommandType::ChangeDirectory:
            if (command.args.empty() || !fileSystem.changeDirectory(joinArgs(command.args))) {
                std::cout << "Directory not found.\n";
            }
            break;
        case CommandType::PrintWorkingDirectory:
            std::cout << fileSystem.currentPath().string() << '\n';
            break;
        case CommandType::Search: {
            if (command.args.empty()) {
                std::cout << "Usage: search <name>\n";
                break;
            }

            const auto results = fileSystem.search(joinArgs(command.args));
            for (const auto& result : results) {
                std::cout << (result.isDirectory ? "[D] " : "[F] ") << result.path.string() << '\n';
            }
            std::cout << results.size() << " result(s)\n";
            break;
        }
        case CommandType::Info: {
            if (command.args.empty()) {
                std::cout << "Usage: info <path>\n";
                break;
            }

            const auto file = fileSystem.fileInfo(joinArgs(command.args));
            std::cout << "Name: " << file.name() << '\n'
                      << "Path: " << file.path().string() << '\n'
                      << "Size: " << Utility::formatFileSize(file.size()) << '\n'
                      << "Hidden: " << (file.isHidden() ? "yes" : "no") << '\n'
                      << "Modified: " << file.lastWriteTime() << '\n';
            break;
        }
        case CommandType::Help:
            printHelp();
            break;
        case CommandType::Exit:
            storage.saveLastPath(fileSystem.currentPath());
            return 0;
        case CommandType::Invalid:
            std::cout << "Unknown command. Type help.\n";
            break;
        case CommandType::Clear:
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
            break;
        }
    }

    storage.saveLastPath(fileSystem.currentPath());
    return 0;
}
