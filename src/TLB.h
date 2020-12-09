#ifndef TLB_H
#define TLB_H

#include "ITranslationBuffer.h"
#include "TraceConfig.h"
#include "IPageTable.h"
#include "SwapSubject.h"

#include <functional>
#include <vector>

struct tlbVectorEntry
{
  long long physicalPageNumber;
  long long virtualPageNumber;
  double lastTimeAccessed;
  bool isValid;
};

class TLB: public ITranslationBuffer
{
public:
  TLB(const TraceConfig& config, IPageTable& pt, SwapSubject& swapSubject);
  
  // Methods from interface
  TLBReturnType GetPhysicalAddress(long long virtualAddress, bool isWrite) override;
  long long GetHits() override;
  long long GetMisses() override;
  long long GetPageTableReferences() override;

private:
  const TraceConfig& Config;
  IPageTable& PT;
  void EvictEntry(long long physicalPageNumber);

  // Any additional properties and methods
  long long numHits;
  long long numMisses;
  long long numPTRefs;
  long long numValidEntries;

  void AddEntry(tlbVectorEntry newEntry);
  std::vector<tlbVectorEntry> tlbVector;
  bool entryPresent(long long memory);

  std::function<void(SwapEvent)> GetSwapHandler();
  
  long long CalculatePhysicalAddress(long long physicalPageNumber, long long virtualAddress);
};

#endif
