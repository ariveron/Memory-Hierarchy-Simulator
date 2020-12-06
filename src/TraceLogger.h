#ifndef TRACE_LOGGER_H
#define TRACE_LOGGER_H

#include "TraceConfig.h"

class TraceLogger
{
public:
  TraceLogger(const TraceConfig& config);
  void PrintLog(int virtualAddress, int physicalAddress, bool isTLBHit, bool isPTHit, bool isDCHit);
  void PrintLogHeader();

private:
  const TraceConfig& Config;
};

#endif
