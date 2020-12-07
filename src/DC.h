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
  int Tag;
  double LastAccessTime;
};

class DC: public IDataCache
{
public:
  DC(const TraceConfig& config, SwapSubject& swapSubject);

  // Methods from interface
  DCReturnType GetBlock(int physicalAddress, bool isWrite) override;
  int GetHits() override;
  int GetMisses() override;
  int GetMainMemoryReferences() override;

private:
  const TraceConfig& Config;
  std::vector<std::vector<Line>> Cache; 

  int TotalHits;
  int TotalMisses;
  int MainMemoryReferences;

  void LoadLineFromMainMemory(int physicalAddress);
  void EvictOldestLineFromSet(int physicalAddress);
  bool IsSetFull(int physicalAddress);
  void AccessCacheLine(int physicalAddress, bool isWrite);
  void EvictLineFromCache(int physicalAddress);
  bool IsLineInCache(int physicalAddress);

  void ForEachCacheLineInSet(int index, std::function<void(Line&)> lambda);

  int GetTag(int physicalAddress);
  int GetIndex(int physicalAddress);

  std::function<void(SwapEvent)> GetSwapHandler();

  // For debuging
  void PrintCache(const char* msg = "");
  void PrintSet(int index, const char* msg = "");
  void PrintTagAndIndex(int tag, int index, const char* msg = "");
};

#endif
