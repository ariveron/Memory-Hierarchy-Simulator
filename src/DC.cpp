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
  auto emptyLine = Line { true, -1, 0 };
  // A set contains a set number of lines
  auto emptySet = std::vector<Line> { static_cast<size_t>(Config.DataCacheSetSize), emptyLine };
  // The cache is initialized with the specified number of empty sets
  Cache = std::vector<std::vector<Line>> (static_cast<size_t>(Config.DataCacheSets), emptySet);
}

std::function<void(SwapEvent)> DC::GetSwapHandler()
{
  return [&](SwapEvent swapEvent) {
    // If PA is not in cache then nothing needs to be done
    if (!IsInCache(swapEvent.PAEvictedFromMainMemory)) return;
    
    // TODO - move to EvictLineFromSet(int physicalAddress) method
    // If it is in cache we need to remove it
    auto index = GetIndex(swapEvent.PAEvictedFromMainMemory);
    auto tag = GetTag(swapEvent.PAEvictedFromMainMemory); 

    // Get the set that corresponds with the index
    auto indexedSet = Cache.at(index);

    // Look through each line in the set to find the corresponding line
    for (auto line = indexedSet.begin(); line < indexedSet.end(); line++)
    {
      if (line->Tag == tag) 
      {
        // Set it to empty to remove it
        line->IsEmpty = true;
      }
    }
  };
}

DCReturnType DC::GetBlock(int physicalAddress)
{
  auto isCacheHit = IsInCache(physicalAddress); 

  if (isCacheHit)
  {
    TotalHits++;

    // TODO - UpdateLineLastAccessTime(int physicalAddress)
    // TODO - use this ==> double now = std::chrono::system_clock::now().time_since_epoch().count();
    // Update LastAccessTime to now
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

bool DC::IsInCache(int physicalAddress)
{
  // Calculate the index and tag, offset is not needed for simulation
  auto index = GetIndex(physicalAddress);
  auto tag = GetTag(physicalAddress); 

  // Get the set that corresponds with the index
  auto indexedSet = Cache.at(index);

  // Look through each line in the set to see if the tag corresponds, and return true if it does
  for (auto line = indexedSet.begin(); line < indexedSet.end(); line++)
  {
    if (!line->IsEmpty && line->Tag == tag) return true;
  }

  // If tag wasn't found then it is not present in the cache
  return false;
}

int DC::GetTag(int physicalAddress)
{
  auto tagBits = Config.BitsDataCacheTag;
  auto idxBits = Config.BitsDataCacheIndex;
  auto offBits = Config.BitsDataCacheOffset;

  return (physicalAddress & ((2 << (tagBits + idxBits + offBits - 1)) - 1)) >> (idxBits + offBits);
}

int DC::GetIndex(int physicalAddress)
{
  auto idxBits = Config.BitsDataCacheIndex;
  auto offBits = Config.BitsDataCacheOffset;

  return (physicalAddress & ((2 << (idxBits + offBits - 1)) - 1)) >> (offBits);
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
