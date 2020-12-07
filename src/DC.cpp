#include "DC.h"
#include "IDataCache.h"
#include "TraceConfig.h"
#include "SwapSubject.h"

#include <functional>
#include <vector>
#include <chrono>
#include <limits>

#include <iostream>

DC::DC(const TraceConfig& config, SwapSubject& swapSubject)
  : Config { config }, TotalHits { 0 }, TotalMisses { 0 }, MainMemoryReferences { 0 }
{
  // Subscribe to swap events
  swapSubject.Subscribe(GetSwapHandler());

  // Fill an empty set with empty Lines
  Line emptyLine {};
  emptyLine.IsValid = false;
  emptyLine.IsDirty = false;
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
  }
  else
  {
    // Update internal count
    TotalMisses++;
    // Load line from main memory into the data cache
    LoadLineFromMainMemory(physicalAddress);
  }

  // Process the data query
  AccessCacheLine(physicalAddress, isWrite);

  return DCReturnType { isCacheHit };
}

void DC::LoadLineFromMainMemory(int physicalAddress)
{
  // One memory reference to read from memory into cache
  MainMemoryReferences++;

  // Evict oldest line from set if set is full
  if (IsSetFull(physicalAddress))
  {
    EvictOldestLineFromSet(physicalAddress);
  }

  // Get index to find set 
  auto index = GetIndex(physicalAddress);

  // Find an available entry within the data cache to add line
  auto newLineAdded = false;
  ForEachCacheLineInSet(index, [&](Line& line) {
        if (!line.IsValid && !newLineAdded)
        {
          // Set the new line read in from memory
          line.IsValid = true;
          line.IsDirty = false;
          line.Tag = GetTag(physicalAddress);
          std::chrono::system_clock::now().time_since_epoch().count();

          newLineAdded = true;
        }
      });
}

void DC::EvictOldestLineFromSet(int physicalAddress)
{
  // Get the index for findin the sest
  auto index = GetIndex(physicalAddress);

  // Kep track of oldest line in set
  auto oldestLineLastAccessTime = std::numeric_limits<double>::max(); 
  auto oldestLineTag = -1;

  // Find the oldest line tag
  ForEachCacheLineInSet(index, [&](Line& line) {
        if (line.LastAccessTime < oldestLineLastAccessTime)
        {
          oldestLineTag = line.Tag;
          oldestLineLastAccessTime = line.LastAccessTime;
        }
      });
  
  // Write oldest line back to main memory if dirty and invalidate
  ForEachCacheLineInSet(index, [&](Line& line) {
        if (line.IsValid && line.Tag == oldestLineTag) 
        {
          if (line.IsDirty)
          {
            MainMemoryReferences++;
          }

          line.IsValid = false;
        }
      });
}

bool DC::IsSetFull(int physicalAddress)
{
  bool isSetfull = true;

  // Get index and tag for identifying cache line
  auto index = GetIndex(physicalAddress);

  // If at least one line is not valid then set is not full
  ForEachCacheLineInSet(index, [&](Line& line) {
        if (!line.IsValid) isSetfull = false;
      });

  return isSetfull;
}

void DC::AccessCacheLine(int physicalAddress, bool isWrite)
{
  // Get index and tag for identifying cache line
  auto index = GetIndex(physicalAddress);
  auto tag = GetTag(physicalAddress);

  // Update cache line and apply write policy if writing
  ForEachCacheLineInSet(index, [&](Line& line) {
        if (line.Tag == tag)
        {
          double currentTime = std::chrono::system_clock::now().time_since_epoch().count();
          line.LastAccessTime = currentTime;

          if (isWrite && Config.DataCacheWriteThrough)
          {
            MainMemoryReferences++;
            line.IsDirty = false;
          }
          else if (isWrite)
          {
            line.IsDirty = true;
          }
        }
      });
}

void DC::EvictLineFromCache(int physicalAddress)
{
  // Get index and tag for identifying cache line
  auto index = GetIndex(physicalAddress);
  auto tag = GetTag(physicalAddress);

  // Invalidate the cache line if it matches the tag within the set
  // and write to main memory if dirty
  ForEachCacheLineInSet(index, [&](Line& line) {
        if (line.Tag == tag) 
        {
          if (line.IsDirty) MainMemoryReferences++; 

          line.IsValid = false;
        }
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
  auto& indexedSet = Cache.at(index);

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

void DC::PrintCache(const char* msg)
{
  for (size_t i = 0; i < Cache.size(); i++)
  {
    PrintSet(i);
  }
}

void DC::PrintSet(int index, const char* msg)
{
  auto& cacheSet = Cache.at(index);

  for (auto line = cacheSet.begin(); line != cacheSet.end(); line++)
  {
    PrintTagAndIndex(line->Tag, index);
  }
}

void DC::PrintTagAndIndex(int tag, int index, const char* msg)
{
  std::cout << msg << "tag=" << tag << " index=" << index << std::endl;
}
