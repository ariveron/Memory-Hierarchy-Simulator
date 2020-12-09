#ifndef I_DATA_CACHE_H
#define I_DATA_CACHE_H

struct DCReturnType
{
  bool DCHit;
};

class IDataCache
{
public:
  IDataCache() = default;
  virtual ~IDataCache() = default;
  virtual DCReturnType GetBlock(long long physicalAddress, bool isWrite) = 0;
  virtual long long GetHits() = 0;
  virtual long long GetMisses() = 0;
  virtual long long GetMainMemoryReferences() = 0;
};

#endif
