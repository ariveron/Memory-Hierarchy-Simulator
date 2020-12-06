#include "TLB.h"

#include "ITranslationBuffer.h"
#include "TraceConfig.h"
#include "IPageTable.h"
#include "SwapSubject.h"

#include <functional>

TLB::TLB(const TraceConfig& config, IPageTable& pt, SwapSubject& swapSubject)
  : Config { config }, PT { pt }
{
  // Subscribe to swap events
  swapSubject.Subscribe(GetSwapHandler());

  // TODO
  // Any additional intialization
}

std::function<void(SwapEvent)> TLB::GetSwapHandler()
{
  return [&](SwapEvent swapEvent) {
    // TODO
    // Gets called when a swap happens in the PT
    // Has access to all properties of this object
    // Do here whatever you need to do if a page got
    // evicted from main memory that the TLB was caching
    // You can see what page got evicted in the swapEvent
  };
}

TLBReturnType TLB::GetPhysicalAddress(int virtualAddress)
{
  // TODO
  TLBReturnType returnType;
  return returnType;
}

int TLB::GetHits()
{
  // TODO
  return 0;
}

int TLB::GetMisses()
{
  // TODO
  return 0;
}

int TLB::GetPageTableReferences()
{
  // TODO
  return 0;
}
