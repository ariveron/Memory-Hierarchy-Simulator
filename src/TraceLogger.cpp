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
  std::cout <<
    "Virtual  Virt.  Page TLB  PT   Phys        DC  DC  \n"
    "Address  Page # Off. Res. Res. Pg # DC Tag Idx Res.\n"
    "-------- ------ ---- ---- ---- ---- ------ --- ----\n"
    << std::flush;
}

void TraceLogger::PrintLogFooter() const
{
  std::cout << std::endl;
}
