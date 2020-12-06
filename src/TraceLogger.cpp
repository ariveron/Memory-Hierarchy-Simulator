#include "TraceLogger.h"
#include "TraceConfig.h"

#include <iostream>

TraceLogger::TraceLogger(const TraceConfig& config)
  : Config { config }
{}

void TraceLogger::PrintLog(int virtualAddress, int physicalAddress, bool isTLBHit, bool isPTHit, bool isDCHit) const
{
  // TODO
  std::cout 
    << "VA=" << virtualAddress << " PA=" << physicalAddress
    << " TLBHit=" << isTLBHit << " PTHit=" << isPTHit << " DCHit=" << isDCHit
    << std::endl;
}

void TraceLogger::PrintLogHeader() const
{
  // TODO
  std::cout << "Log Header" << std::endl;
}

void TraceLogger::PrintLogFooter() const
{
  // TODO
  std::cout << std::endl;
}
