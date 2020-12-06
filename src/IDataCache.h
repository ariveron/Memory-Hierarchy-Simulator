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
  virtual DCReturnType GetBlock(int physicalAddress) = 0;
  virtual int GetHits() = 0;
  virtual int GetMisses() = 0;
  virtual int GetMainMemoryReferences() = 0;
};

#endif
