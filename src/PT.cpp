#include "PT.h"
#include "IPageTable.h"
#include "TraceConfig.h"
#include "SwapSubject.h"

PT::PT(const TraceConfig& config, SwapSubject& swapSubject)
  : Config { config }, Swap {swapSubject }
{
  // TODO
  // Any additional initialization
}

PTReturnType PT::GetPhysicalAddress(int virtualAddress, bool isWrite)
{
  // TODO
  PTReturnType returnType;
  return returnType;
}

int PT::GetHits()
{
  // TODO
  return 0;
}

int PT::GetFaults()
{
  // TODO
  return 0;
}

int PT::GetDiskReferences()
{
  // TODO
  return 0;
}
