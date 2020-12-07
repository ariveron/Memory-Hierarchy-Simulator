#include "DC.h"
#include "IDataCache.h"
#include "TraceConfig.h"
#include "SwapSubject.h"

#include <functional>
#include <vector>

DC::DC(const TraceConfig& config, SwapSubject& swapSubject)
  : Config { config }, TotalHits { 0 }, TotalMisses { 0 }, MainMemoryReferences { 0 }
{
  // Subscribe to swap events
  swapSubject.Subscribe(GetSwapHandler());

  // Fill an empty set with empty Lines
  Line emptyLine {};
  emptyLine.IsValid = false;
  emptyLine.Tag = -1;
  emptyLine.LastAccessTime = 0;
  // A set contains a set number of lines
  auto emptySet = std::vector<Line> { static_cast<size_t>(Config.DataCacheSetSize), emptyLine };
  // The cache is initialized with the specified number of empty sets
  Cache = std::vector<std::vector<Line>> (static_cast<size_t>(Config.DataCacheSets), emptySet);
}

std::function<void(SwapEvent)> DC::GetSwapHandler()
{
  return [&](SwapEvent swapEvent) {
    // Nothing to do if line is not present
    if (!IsLineInCache(swapEvent.PAEvictedFromMainMemory)) return;

    // Evict address from cache if present
    EvictLineFromCache(swapEvent.PAEvictedFromMainMemory); 
  };
}

DCReturnType DC::GetBlock(int physicalAddress, bool isWrite)
{
  auto isCacheHit = IsLineInCache(physicalAddress); 

  if (isCacheHit)
  {
    // Update internal count
    TotalHits++;
    // Process the data query
    AccessCacheLine(physicalAddress, isWrite);
  }
  else
  {
    TotalMisses++;
    MainMemoryReferences++;

    // TODO
    // Place line in an empty line in the set TODO - AddLineToSet(int physicalAddress) can use methods below
    // TODO - bool SetContainsEmptyLine(int physicalAddress);
    // Evict a line if needed from the set TODO - EvictOldestLineFromSet(int physicalAddress)
    // Remember to update last access time to now
  }

  return DCReturnType { isCacheHit };
}

void DC::AccessCacheLine(int physicalAddress, bool isWrite)
{
  auto currentTime = std::chrono::system_clock::now().time_since_epoch().count();

  // TODO
}

void DC::EvictLineFromCache(int physicalAddress)
{
  // Get index and tag for identifying cache line
  auto index = GetIndex(physicalAddress);
  auto tag = GetTag(physicalAddress);

  // Invalidate the cache line if it matches the tag within the set
  ForEachCacheLineInSet(index, [tag](Line& line) {
        if (line.Tag == tag) line.IsValid = false;
      });
}

bool DC::IsLineInCache(int physicalAddress)
{
  auto isFound = false;
  
  // Get index and tag for identifying cache line
  auto index = GetIndex(physicalAddress);
  auto tag = GetTag(physicalAddress); 

  // Try to find the tag within the set
    ForEachCacheLineInSet(index, [&isFound, tag](Line& line) {
        if (line.IsValid && line.Tag == tag) isFound = true; 
      });
  
  return isFound;
}

void DC::ForEachCacheLineInSet(int index, std::function<void(Line&)> lambda)
{
  // Get the set that corresponds with the index
  auto indexedSet = Cache.at(index);

  // Loop through each line in the set and apply the lambda
  for (auto line = indexedSet.begin(); line < indexedSet.end(); line++)
  {
     lambda(*line);
  }
}

int DC::GetIndex(int physicalAddress)
{
  auto idxBits = Config.BitsDataCacheIndex;
  auto offBits = Config.BitsDataCacheOffset;

  return (physicalAddress & ((2 << (idxBits + offBits - 1)) - 1)) >> (offBits);
}

int DC::GetTag(int physicalAddress)
{
  auto tagBits = Config.BitsDataCacheTag;
  auto idxBits = Config.BitsDataCacheIndex;
  auto offBits = Config.BitsDataCacheOffset;

  return (physicalAddress & ((2 << (tagBits + idxBits + offBits - 1)) - 1)) >> (idxBits + offBits);
}

int DC::GetHits()
{
  return TotalHits;
}

int DC::GetMisses()
{
  return TotalMisses;
}

int DC::GetMainMemoryReferences()
{
  return MainMemoryReferences;
}
