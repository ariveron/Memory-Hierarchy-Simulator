#ifndef PT_H
#define PT_H

#include "IPageTable.h"
#include "TraceConfig.h"
#include "SwapSubject.h"

class PT: public IPageTable
{
public:
  PT(const TraceConfig& config, SwapSubject& swapSubject);

  // Methods from interface
  PTReturnType GetPhysicalAddress(int virtualAddress) override;
  int GetHits() override;
  int GetFaults() override;
  int GetDiskReferences() override;

private:
  const TraceConfig& Config;
  SwapSubject& Swap;

  // TODO
  // Any additional properties and methods
};

#endif
