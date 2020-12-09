#ifndef TRACE_LOGGER_H
#define TRACE_LOGGER_H

#include "TraceConfig.h"

class TraceLogger
{
public:
  TraceLogger(const TraceConfig& config);
  void PrintLog(long long virtualAddress, long long physicalAddress, bool isTLBHit, bool isPTHit, bool isDCHit) const;
  void PrintLogHeader() const;
  void PrintLogFooter() const;

private:
  const TraceConfig& Config;
};

#endif
