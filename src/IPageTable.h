#ifndef I_PAGE_TABLE_H
#define I_PAGE_TABLE_H

struct PTReturnType 
{
  long long PhysicalAddress;
  bool PTHit; 
};

class IPageTable
{
public:
  IPageTable() = default;
  virtual ~IPageTable() = default;
  virtual PTReturnType GetPhysicalAddress(long long virtualAddress, bool isWrite) = 0;
  virtual long long GetHits() = 0;
  virtual long long GetFaults() = 0;
  virtual long long GetDiskReferences() = 0;
  virtual void SetDirtyFlag(long long physticalAddress) = 0;
};

#endif
