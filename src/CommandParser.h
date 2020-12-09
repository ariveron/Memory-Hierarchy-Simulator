#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <map>
#include <string>
#include <tuple>

class CommandParser
{
public:
  CommandParser(long long argc, char** argv, std::string delimiter);
  bool IsCommandPresent(std::string command);
  bool IsCommandEmpty(std::string command);
  std::string GetCommand(std::string command);
  std::tuple<bool,int> TryGetCommandAsInt(std::string command);

private:
  std::map<std::string, std::string> commands_;
  bool IsAnArg_(const char* arg, std::string delimiter);
};

#endif
