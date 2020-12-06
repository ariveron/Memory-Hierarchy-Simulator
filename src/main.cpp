#include "CommandParser.h"
#include "TraceConfig.h"
#include "Trace.h"
#include "TraceLogger.h"

#include <iostream>

int main(int argc, char* argv[])
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

  // Parse the trace.config file and output configuration
  TraceConfig config { commandParser.GetCommand("c") };
  std::cout <<
    "Number of virtual pages is " << config.PageTableVirtualPages << ".\n"
    "Number of physical pages is " << config.PageTablePhysicalPages << ".\n"
    "Each page contains " << config.PageTablePageSize << " bytes.\n"
    "Number of bits used for the page table index is " << config.BitsPageTableIndex << ".\n"
    "Number of bits used for the page offset is " << config.BitsPageTableOffset << ".\n"
    "\n"
    "Data TLB contains " << config.TLBEntries << " entries.\n"
    "\n"
    "Data cache contains " << config.DataCacheSets << " sets.\n"
    "Each set contains " << config.DataCacheSetSize << " entries.\n"
    "Each line is " << config.DataCacheLineSize << " bytes.\n"
    "The cache " 
      << (config.DataCacheWriteThrough ? "does not use" : "uses")
      << " a write-allocate and write-back policy.\n"
    "Number of bits used for the tags is " << config.BitsDataCacheTag << ".\n"
    "Number of bits used for the index is " << config.BitsDataCacheIndex << ".\n"
    "Number of bits used for the offset is " << config.BitsDataCacheOffset << ".\n"
    "\n"
    "The addresses read in are " << (config.UseVirtualAddreses ? "" : "not ") << "virtual addresses.\n"
    "The translation lookaside buffer is " << (config.UseTLB ? "enabled" : "disabled") << ".\n"
    "\n"
    << std::flush;

  // Output table header
  TraceLogger logger { config };
  logger.PrintLogHeader();
  
  // Run simulation and output results
  Trace trace { commandParser.GetCommand("t") };
  while (!trace.IsDone()) 
  {
    auto entry = trace.Next();
    logger.PrintLog(entry.Address, entry.Address, true, true, true);
  }
  std::cout << std::endl;
  
  // Statistics table header
  std::cout <<
    "Statistic Table Header\n"
    "\n"
    << std::flush;

  exit(0);
}
