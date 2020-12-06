#include "DC.h"
#include "IDataCache.h"
#include "TraceConfig.h"
#include "SwapSubject.h"

#include <functional>

DC::DC(const TraceConfig& config, SwapSubject& swapSubject)
  : Config { config }
{
  // Subscribe to swap events
  swapSubject.Subscribe(GetSwapHandler());

  // TODO
  // Any additional initialization
}

std::function<void(SwapEvent)> DC::GetSwapHandler()
{
  return [&](SwapEvent swapEvent) {
    // TODO
    // Gets called when a swap happens in the PT
    // Has access to all properties of this object
    // Do here whatever you need to do if a page got
    // eviceted from main memory that the DC was caching
    // You can see what page got evicted in the swapEvent
  };
}

DCReturnType DC::GetBlock(int physicalAddress)
{
  // TODO
  DCReturnType returnType;
  return returnType;
}

int DC::GetHits()
{
  // TODO
  return 0;
}

int DC::GetMisses()
{
  // TODO
  return 0;
}

int DC::GetMainMemoryReferences()
{
  // TODO
  return 0;
}
