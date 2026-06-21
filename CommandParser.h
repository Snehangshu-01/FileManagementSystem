#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <vector>

enum class CommandType {
    List,
    ChangeDirectory,
    PrintWorkingDirectory,
    Search,
    Info,
    Help,
    Clear,   
    Exit,
    Open,    
    Invalid
};
struct Command {
    CommandType type = CommandType::Invalid;
    std::vector<std::string> args;
};

class CommandParser {
public:
    Command parse(const std::string& input) const;

private:
    static std::vector<std::string> tokenize(const std::string& input);
};

#endif
