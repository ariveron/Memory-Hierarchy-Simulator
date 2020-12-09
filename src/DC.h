#ifndef DC_H
#define DC_H

#include "IDataCache.h"
#include "TraceConfig.h"
#include "SwapSubject.h"

#include <functional>
#include <vector>

struct Line
{
  bool IsValid;
  bool IsDirty;
  long long Tag;
  double LastAccessTime;
};

class DC: public IDataCache
{
public:
  DC(const TraceConfig& config, SwapSubject& swapSubject);

  // Methods from interface
  DCReturnType GetBlock(long long physicalAddress, bool isWrite) override;
  long long GetHits() override;
  long long GetMisses() override;
  long long GetMainMemoryReferences() override;

private:
  const TraceConfig& Config;
  std::vector<std::vector<Line>> Cache; 

  long long TotalHits;
  long long TotalMisses;
  long long MainMemoryReferences;

  void LoadLineFromMainMemory(long long physicalAddress);
  void EvictOldestLineFromSet(long long physicalAddress);
  bool IsSetFull(long long physicalAddress);
  void AccessCacheLine(long long physicalAddress, bool isWrite);
  void EvictLineFromCache(long long physicalAddress);
  bool IsLineInCache(long long physicalAddress);

  void ForEachCacheLineInSet(long long index, std::function<void(Line&)> lambda);

  long long GetTag(long long physicalAddress);
  long long GetIndex(long long physicalAddress);

  std::function<void(SwapEvent)> GetSwapHandler();

  // For debuging
  void PrintCache(const char* msg = "");
  void PrintSet(long long index, const char* msg = "");
  void PrintTagAndIndex(long long tag, long long index, const char* msg = "");
};

#endif
