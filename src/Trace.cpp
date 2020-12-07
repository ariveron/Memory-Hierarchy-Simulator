#include "Trace.h"
#include "StringManipulation.h"

#include <fstream>
#include <string>
#include <exception>

#include <sstream>

Trace::Trace(const std::string& traceFileName)
  : TraceFile{ traceFileName }
{
  if (!TraceFile.is_open())
  {
    throw std::runtime_error("Could not open trace file");
  }

  GetNextEntry(); 
}

TraceEntry Trace::Peek()
{
  return NextEntry;
}

TraceEntry Trace::Next()
{
  auto entry = NextEntry;

  GetNextEntry(); 

  return entry;
}

bool Trace::IsDone()
{
  return TraceFile.eof();
}

void Trace::GetNextEntry()
{
  std::string line;
  if (std::getline(TraceFile, line)
      && !line.empty())
  {
    trim(line);
    toupper(line);
    try
    {
      NextEntry.IsWrite = line[0] == 'W';
      NextEntry.Address = std::stol(line.substr(2), nullptr, 16);
    }
    catch (std::exception& e)
    {
      std::stringstream ss {};
      ss << "Unable to parse trace entry " << line;
      throw std::runtime_error(ss.str());
    }
  }
}
