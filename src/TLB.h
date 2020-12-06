#ifndef TLB_H
#define TLB_H

#include "ITranslationBuffer.h"
#include "TraceConfig.h"
#include "IPageTable.h"
#include "SwapSubject.h"

#include <functional>

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
  
  std::function<void(SwapEvent)> GetSwapHandler();

  // TODO
  // Any additional properties and methods
};

#endif
