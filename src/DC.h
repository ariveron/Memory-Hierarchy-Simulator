#ifndef DC_H
#define DC_H

#include "IDataCache.h"
#include "TraceConfig.h"
#include "SwapSubject.h"

#include <functional>

class DC: public IDataCache
{
public:
  DC(const TraceConfig& config, SwapSubject& swapSubject);

  // Methods from interface
  DCReturnType GetBlock(int physicalAddress) override;
  int GetHits() override;
  int GetMisses() override;
  int GetMainMemoryReferences() override;

private:
  const TraceConfig& Config;

  std::function<void(SwapEvent)> GetSwapHandler();

  // TODO
  // Any additional properties and methods
};

#endif
