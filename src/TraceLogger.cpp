#include "TraceLogger.h"
#include "TraceConfig.h"

#include <iostream>
#include <iomanip>

TraceLogger::TraceLogger(const TraceConfig& config)
  : Config { config }
{}

void TraceLogger::PrintLog(int virtualAddress, int physicalAddress, bool isTLBHit, bool isPTHit, bool isDCHit) const
{
  auto virtualPageNumber = virtualAddress >> Config.BitsPageTableOffset;
  auto pageOffset = virtualAddress & ((1 << Config.BitsPageTableOffset) - 1);
  auto physicalPageNumber = (physicalAddress >> Config.BitsPageTableOffset);
  auto dataCacheTag = (physicalAddress & ((1 << (Config.BitsDataCacheTag + Config.BitsDataCacheIndex + Config.BitsDataCacheOffset)) - 1))
    >> (Config.BitsDataCacheIndex + Config.BitsDataCacheOffset);
  auto dataCacheIndex = (physicalAddress & ((1 << (Config.BitsDataCacheIndex + Config.BitsDataCacheOffset)) - 1))
    >> (Config.BitsDataCacheOffset);

  std::cout
    << std::setw(8) << std::hex << std::setfill('0') << virtualAddress << std::setw(1) << std::setfill(' ') << " "
    << std::setw(6) << std::hex << virtualPageNumber << std::setw(1) << " "
    << std::setw(4) << std::hex << pageOffset << std::setw(1) << " "
    << std::setw(4) << (!Config.UseVirtualAddreses || !Config.UseTLB ? "" : (isTLBHit ? "hit" : "miss")) << std::setw(1) << " "
    << std::setw(4) << (!Config.UseVirtualAddreses || isTLBHit ? "" : (isPTHit ? "hit" : "miss")) << std::setw(1) << " "
    << std::setw(4) << std::hex << physicalPageNumber << std::setw(1) << " "
    << std::setw(6) << std::hex << dataCacheTag << std::setw(1) << " "
    << std::setw(3) << std::hex << dataCacheIndex << std::setw(1) << " "
    << std::setw(4) << (isDCHit ? "hit" : "miss") << std::setw(1) << " "
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
