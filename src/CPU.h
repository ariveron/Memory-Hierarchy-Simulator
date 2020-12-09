#ifndef CPU_H
#define CPU_H

#include "TraceConfig.h"
#include "TraceLogger.h"
#include "Trace.h"
#include "ITranslationBuffer.h"
#include "IPageTable.h"
#include "IDataCache.h"

class CPU
{
public:
  CPU(const TraceConfig& config, const TraceLogger& logger,
      ITranslationBuffer& tlb, IPageTable& pt, IDataCache& dc);
  void Run(Trace& trace);
  void PrintStatistics();

private:
  const TraceConfig& Config;
  const TraceLogger& Logger;
  ITranslationBuffer& TLB;
  IPageTable& PT;
  IDataCache& DC;
  int TotalReads;
  int TotalWrites;
  int PageTableReferences;
};

#endif
