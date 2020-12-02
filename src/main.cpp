#include "CommandParser.h"

#include <iostream>

int main(int argc, char* argv[])
{
  CommandParser commandParser{ argc, argv, "--" };

  if (commandParser.IsCommandPresent("help")
      || commandParser.IsCommandEmpty("c")
      || commandParser.IsCommandEmpty("t"))
  {
    std::cout << 
      "\n"
      "\tUsage: " << argv[0] << " --c trace.config --t trace.dat\n"
      "\n"
      "\t\t --c \t\tfile path to trace configuration file\n"
      "\t\t --t \t\tfile path to trace file\n"
      << std::endl;

    exit(-1);
  }

  exit(0);
}
