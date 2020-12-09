#ifndef TRACE_CONFIG_H
#define TRACE_CONFIG_H

#include <string>

class TraceConfig
{
public:
  const static long long TLBMaxEntries;
  const static long long PageTableMaxVirtualPages;
  const static long long PageTableMaxPhysicalPages;
  const static long long DataCacheMaxSets;
  const static long long DataCacheMaxSetSize;
  const static long long DataCacheMinLineSize;

  long long TLBEntries;
  long long PageTableVirtualPages;
  long long PageTablePhysicalPages;
  long long PageTablePageSize;
  long long DataCacheTotalines;
  long long DataCacheSets;
  long long DataCacheSetSize;
  long long DataCacheLineSize;
  bool DataCacheWriteThrough;
  bool UseVirtualAddreses;
  bool UseTLB;

  long long MainMemorySize;
  long long BitsPageTableOffset;
  long long BitsPageTableIndex;
  long long BitsDataCacheTag;
  long long BitsDataCacheIndex;
  long long BitsDataCacheOffset;

  TraceConfig(const std::string& configFileName);
  void PrintConfiguration() const;

private:
  const static std::string HeaderDataTLBConfiguration;
  const static std::string HeaderPageTableConfiguration;
  const static std::string HeaderDataCacheConfiguration;
  const static std::string HeaderNoHeader;

  const static std::string FieldDelimeter;
  const static std::string FieldValueTrue;

  const static std::string FieldNumberOfEntries;
  const static std::string FieldNumberOfVirtualPages;
  const static std::string FieldNumberOfPhysicalPages;
  const static std::string FieldPageSize;
  const static std::string FieldSetSize;
  const static std::string FieldLineSize;
  const static std::string FieldWriteThrough;
  const static std::string FieldVirtualAddresses;
  const static std::string FieldTLB;

  bool IsAField(const std::string& line);
  std::string GetHeader(const std::string& line);
  std::string GetFieldKey(const std::string& line);
  std::string GetFieldVal(const std::string& line);
};

#endif
