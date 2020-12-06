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

  while (!trace.IsDone())
  {
    auto entry = trace.Next();

    if (entry.IsWrite) TotalWrites++;
    else TotalReads++;

    

    Logger.PrintLog(entry.Address, entry.Address, true, true, true); 
  }

  Logger.PrintLogFooter();
}

void CPU::PrintStatistics()
{
  // TODO
  std::cout << "Statistics Go Here" << std::endl;
}
