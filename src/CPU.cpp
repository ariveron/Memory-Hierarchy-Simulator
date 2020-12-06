#include "CPU.h"
#include "TraceConfig.h"
#include "TraceLogger.h"
#include "Trace.h"
#include "ITranslationBuffer.h"
#include "IPageTable.h"
#include "IDataCache.h"

#include <iostream>

CPU::CPU(const TraceConfig& config, const TraceLogger& logger,
    ITranslationBuffer& tlb, IPageTable& pt, IDataCache& dc)
  : Config { config }, Logger (logger), TotalReads { 0 }, TotalWrites { 0 },
  TLB { tlb }, PT { pt }, DC { dc }
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
      auto tlbReturn = TLB.GetPhysicalAddress(entry.Address);
      physicalAddress = tlbReturn.PhysicalAddress;
      isTLBHit = tlbReturn.TLBHit;
      isPTHit = tlbReturn.PTHit;
    }
    // Make query to PT if using VAs but not TLB
    else if (Config.UseVirtualAddreses)
    {
      auto ptReturn = PT.GetPhysicalAddress(entry.Address);
      physicalAddress = ptReturn.PhysicalAddress;
      isTLBHit = false;
      isPTHit = ptReturn.PTHit;
    }
    // If not using VA then the entry address is the PA
    else
    {
      physicalAddress = entry.Address;
      isTLBHit = false;
      isPTHit = false;
    }
    
    // Get info from DC about memory access
    bool isDCHit = DC.GetBlock(physicalAddress).DCHit;

    // Log output of processing this trace entry
    Logger.PrintLog(entry.Address, physicalAddress, isTLBHit, isPTHit, isDCHit); 
  }

  Logger.PrintLogFooter();
}

void CPU::PrintStatistics()
{
  // TODO
  std::cout << "Statistics Go Here" << std::endl;
}
