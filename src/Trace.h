#ifndef TRACE_H
#define TRACE_H

#include <fstream>
#include <string>

struct TraceEntry
{
  bool IsWrite;
  int Address;
};

class Trace
{
public:
  Trace(const std::string& traceFileName);
  TraceEntry Peek();
  TraceEntry Next();
  bool IsDone();

private:
  TraceEntry NextEntry;
  std::ifstream TraceFile;
  void GetNextEntry();
};

#endif
