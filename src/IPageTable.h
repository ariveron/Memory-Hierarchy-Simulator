#ifndef I_PAGE_TABLE_H
#define I_PAGE_TABLE_H

struct PTReturnType 
{
  int PhysicalAddress;
  bool PTHit; 
};

class IPageTable
{
public:
  IPageTable() = default;
  virtual ~IPageTable() = default;
  virtual PTReturnType GetPhysicalAddress(int virtualAddress) = 0;
  virtual int GetHits() = 0;
  virtual int GetFaults() = 0;
  virtual int GetDiskReferences() = 0;
};

#endif
