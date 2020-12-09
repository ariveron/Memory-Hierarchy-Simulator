#include "CPU.h"
#include "TraceConfig.h"
#include "TraceLogger.h"
#include "Trace.h"
#include "ITranslationBuffer.h"
#include "IPageTable.h"
#include "IDataCache.h"

#include <iostream>
#include <iomanip>

CPU::CPU(const TraceConfig& config, const TraceLogger& logger,
    ITranslationBuffer& tlb, IPageTable& pt, IDataCache& dc)
  : Config { config }, Logger (logger), TotalReads { 0 }, TotalWrites { 0 },
  TLB { tlb }, PT { pt }, DC { dc }, PageTableReferences { 0 }
{}

void CPU::Run(Trace& trace)
{
  Config.PrintConfiguration();

  Logger.PrintLogHeader();

  // keep going until trace has no more entries
  while (!trace.IsDone())
  {
    // Get the next trace entry
    auto entry = trace.Next();

    // Keep track of reads and writes
    if (entry.IsWrite) TotalWrites++;
    else TotalReads++;

    // Get the physical address and TLB/PT info
    int physicalAddress;
    bool isTLBHit;
    bool isPTHit;

    // Make query to TLB if using VAs and TLB
    if (Config.UseVirtualAddreses && Config.UseTLB)
    {
      auto tlbReturn = TLB.GetPhysicalAddress(entry.Address, entry.IsWrite);
      physicalAddress = tlbReturn.PhysicalAddress;
      isTLBHit = tlbReturn.TLBHit;
      isPTHit = tlbReturn.PTHit;
    }
    // Make query to PT if using VAs but not TLB
    else if (Config.UseVirtualAddreses)
    {
      auto ptReturn = PT.GetPhysicalAddress(entry.Address, entry.IsWrite);
      physicalAddress = ptReturn.PhysicalAddress;
      isTLBHit = false;
      isPTHit = ptReturn.PTHit;
      PageTableReferences++;
    }
    // If not using VA then the entry address is the PA
    else
    {
      physicalAddress = entry.Address;
      isTLBHit = false;
      isPTHit = false;
    }

    // Make sure that PT is aware of write to a page
    if (isTLBHit && entry.IsWrite) PT.SetDirtyFlag(physicalAddress);

    // Get info from DC about memory access
    bool isDCHit = DC.GetBlock(physicalAddress, entry.IsWrite).DCHit;

    // Log output of processing this trace entry
    Logger.PrintLog(entry.Address, physicalAddress, isTLBHit, isPTHit, isDCHit); 
  }

  Logger.PrintLogFooter();
}

void CPU::PrintStatistics()
{
  auto tlbHitRatio = TLB.GetHits() + TLB.GetMisses() == 0 ? 0 : static_cast<double>(TLB.GetHits()) / (TLB.GetHits() + TLB.GetMisses());
  auto ptHitRatio = PT.GetHits() + PT.GetFaults() == 0 ? 0 : static_cast<double>(PT.GetHits()) / (PT.GetHits() + PT.GetFaults());
  auto dcHitRatio = DC.GetHits() + DC.GetMisses() == 0 ? 0 : static_cast<double>(DC.GetHits()) / (DC.GetHits() + DC.GetMisses());
  auto readRatio = TotalReads + TotalWrites == 0 ? 0 : static_cast<double>(TotalReads) / (TotalReads + TotalWrites);

  std::cout << std::dec << std::fixed << std::setprecision(6) <<
    "Simulation Statistics\n"
    "---------------------\n"
    "\n"
    "Data TLB hits:           " << TLB.GetHits() << "\n"
    "Data TLB misses:         " << TLB.GetMisses() << "\n"
    "Data TLB hit ratio:      " << tlbHitRatio << "\n"
    "\n"
    "Page table hits:         " << PT.GetHits() << "\n"
    "Page table faults:       " << PT.GetFaults() << "\n"
    "Page table hit ratio:    " << ptHitRatio << "\n"
    "\n"
    "Data cache hits:         " << DC.GetHits() << "\n"
    "DC misses:               " << DC.GetMisses() << "\n"
    "DC hit ratio:            " << dcHitRatio << "\n"
    "\n"
    "Total reads:             " << TotalReads << "\n"
    "Total writes:            " << TotalWrites << "\n"
    "Ratio of reads:          " << readRatio << "\n"
    "\n"
    "Main memory references:  " << DC.GetMainMemoryReferences() << "\n"
    "Page table references:   " << TLB.GetPageTableReferences() + PageTableReferences << "\n"
    "Disk references:         " << PT.GetDiskReferences() << "\n"
    "\n"
    << std::flush;
}
