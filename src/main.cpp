#include "CommandParser.h"
#include "TraceConfig.h"
#include "Trace.h"
#include "TraceLogger.h"
#include "CPU.h"
#include "TLB.h"
#include "PT.h"
#include "DC.h"

#include <iostream>

int main(int argc, char* argv[])
{
  try
  {
    // Parse the command line arguments
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

    // Parse the trace.config file
    TraceConfig config { commandParser.GetCommand("c") };
    // Get the memory trace in wrapper class for quick parsing
    Trace trace { commandParser.GetCommand("t") };

    // Create simulation components
    TraceLogger logger { config };
    SwapSubject swapSubject {};
    DC dc { config, swapSubject };
    PT pt { config, swapSubject };
    TLB tlb { config, pt, swapSubject };
    CPU cpu { config, logger, tlb, pt, dc };

    // Run simulation and output results
    cpu.Run(trace);

    // Prlong long statistics
    cpu.PrintStatistics();
  }
  catch(std::exception& e)
  {
    std::cout << "Unhandled exception: " << e.what() << std::endl;
    exit(-1);
  }
    
  exit(0);
}
