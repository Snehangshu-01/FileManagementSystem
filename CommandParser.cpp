#include "CommandParser.h"

#include <sstream>
#include <unordered_map>

Command CommandParser::parse(const std::string& input) const {
    const auto tokens = tokenize(input);
    if (tokens.empty()) {
        return {};
    }

    static const std::unordered_map<std::string, CommandType> commands = {
        {"ls", CommandType::List},
        {"dir", CommandType::List},
        {"cd", CommandType::ChangeDirectory},
        {"pwd", CommandType::PrintWorkingDirectory},
        {"search", CommandType::Search},
        {"find", CommandType::Search},
        {"info", CommandType::Info},
        {"help", CommandType::Help},
        {"exit", CommandType::Exit},
        {"quit", CommandType::Exit},
        {"clear", CommandType::Clear},
        {"cls", CommandType::Clear},
        {"open", CommandType::Open},
    };

    Command command;
    const auto match = commands.find(tokens.front());
    command.type = match == commands.end() ? CommandType::Invalid : match->second;
    command.args.assign(tokens.begin() + 1, tokens.end());
    return command;
}

std::vector<std::string> CommandParser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}
