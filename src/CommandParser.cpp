#include "CommandParser.h"

#include <map>
#include <string>
#include <tuple>

CommandParser::CommandParser(int argc, char** argv, std::string delimiter)
  : commands_{}
{
  // Map out all of the arguments to commands and values
  for (size_t i = 0; i < argc; i++)
  {
    // Store the command without the delimiter 
    if (!IsAnArg_(argv[i], delimiter)) continue;
    std::string command = &(argv[i][delimiter.size()]);

    // If next is not an arg then it's a value
    std::string value{};
    if (i + 1 < argc 
        && !IsAnArg_(argv[i + 1], delimiter))
    {
      value = argv[i + 1];
      i++;
    }

    // Store the command and value
    commands_[command] = value;
  }
}

// Test if command is present.
bool CommandParser::IsCommandPresent(std::string command)
{
  return commands_.find(command) != commands_.end();
}

// Test if command has no value. Returns true if command
// is not present or has an empty string as its value.
bool CommandParser::IsCommandEmpty(std::string command)
{
  auto it = commands_.find(command);
  if (it == commands_.end()) return true;
  return it->second.empty();
}

// Returns the value of the command. Returns an empty string
// if command is not present.
std::string CommandParser::GetCommand(std::string command)
{
  auto it = commands_.find(command);
  if (it == commands_.end()) return std::string{};
  return it->second;
}

// Returns true if arg starts with delimiter.
bool CommandParser::IsAnArg_(const char* arg, std::string delimiter)
{
  size_t delimiterIndex = 0;
  size_t argIndex = 0;

  for (
      ; delimiterIndex < delimiter.size() && arg[argIndex] != '\0'
      ; delimiterIndex++, argIndex++)
  {
    if (delimiter[delimiterIndex] != arg[argIndex]) return false;
  }

  return delimiterIndex == delimiter.size() && arg[argIndex] != '\0';
}

// Returns <bool success, int result> where success is false if
// command doesn't exist, has no value, or value does not convert
// to the result type int
std::tuple<bool,int> CommandParser::TryGetCommandAsInt(std::string command)
{
  if (IsCommandEmpty(command)) return std::make_tuple(false, 0);

  try
  {
    int result = std::stoi(GetCommand(command));
    return std::make_tuple(true, result);
  }
  catch (const std::exception&)
  {
    return std::make_tuple(false, 0);
  }
}
