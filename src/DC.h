#ifndef DC_H
#define DC_H

#include "IDataCache.h"
#include "TraceConfig.h"
#include "SwapSubject.h"

#include <functional>
#include <vector>
#include <chrono>

struct Line
{
  bool IsEmpty;
  int Tag;
  double LastAccessTime;
};

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
  std::vector<std::vector<Line>> Cache; 

  int TotalHits;
  int TotalMisses;
  int MainMemoryReferences;

  Line& GetLine(int physicalAddress);
  bool IsInCache(int physicalAddress);
  int GetTag(int physicalAddress);
  int GetIndex(int physicalAddress);

  std::function<void(SwapEvent)> GetSwapHandler();
};

#endif
