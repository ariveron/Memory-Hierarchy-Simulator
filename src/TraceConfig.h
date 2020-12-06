#ifndef TRACE_CONFIG_H
#define TRACE_CONFIG_H

#include <string>

class TraceConfig
{
public:
  const static int TLBMaxEntries;
  const static int PageTableMaxVirtualPages;
  const static int PageTableMaxPhysicalPages;
  const static int DataCacheMaxSets;
  const static int DataCacheMaxSetSize;
  const static int DataCacheMinLineSize;

  int TLBEntries;
  int PageTableVirtualPages;
  int PageTablePhysicalPages;
  int PageTablePageSize;
  int DataCacheSets;
  int DataCacheSetSize;
  int DataCacheLineSize;
  bool DataCacheWriteThrough;
  bool UseVirtualAddreses;
  bool UseTLB;

  int MainMemorySize;
  int BitsPageTableOffset;
  int BitsPageTableIndex;
  int BitsDataCacheTag;
  int BitsDataCacheIndex;
  int BitsDataCacheOffset;

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
