#ifndef TLB_H
#define TLB_H

#include "ITranslationBuffer.h"
#include "TraceConfig.h"
#include "IPageTable.h"
#include "SwapSubject.h"

#include <functional>
#include <vector>

class TLB: public ITranslationBuffer
{
public:
  TLB(const TraceConfig& config, IPageTable& pt, SwapSubject& swapSubject);
  
  // Methods from interface
  TLBReturnType GetPhysicalAddress(int virtualAddress) override;
  int GetHits() override;
  int GetMisses() override;
  int GetPageTableReferences() override;

private:
  const TraceConfig& Config;
  IPageTable& PT;
  void EvictEntry(int physicalAddress);

  // TODO
  // Any additional properties and methods
  int numHits{};
  int numMisses{};
  int numPTRefs{};

  struct tlbVectorEntry
  {
    int virtualAddress;
    int physicalAddress;
    double lastTimeAccessed;
    bool isValid;
  };
  void AddEntry(tlbVectorEntry newEntry);
  std::vector<tlbVectorEntry> tlbVector;
  bool entryPresent(int memory);

  std::function<void(SwapEvent)> GetSwapHandler();

};

#endif
