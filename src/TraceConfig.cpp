#include "TraceConfig.h"
#include "StringManipulation.h"

#include <string>
#include <fstream>
#include <exception>
#include <map>
#include <cmath>

const int TraceConfig::TLBMaxEntries = 256;
const int TraceConfig::DataCacheMaxSets = 1024;
const int TraceConfig::DataCacheMaxSetSize = 8;
const int TraceConfig::DataCacheMinLineSize = 8;
const int TraceConfig::PageTableMaxVirtualPages = 8192;
const int TraceConfig::PageTableMaxPhysicalPages = 1024;

const std::string TraceConfig::HeaderDataTLBConfiguration = "data tlb configuration";
const std::string TraceConfig::HeaderPageTableConfiguration = "page table configuration";
const std::string TraceConfig::HeaderDataCacheConfiguration = "data cache configuration";
const std::string TraceConfig::HeaderNoHeader = "_";

const std::string TraceConfig::FieldDelimeter = ":";
const std::string TraceConfig::FieldValueTrue = "y";

const std::string TraceConfig::FieldNumberOfEntries = "number of entries";
const std::string TraceConfig::FieldNumberOfVirtualPages = "number of virtual pages";
const std::string TraceConfig::FieldNumberOfPhysicalPages = "number of physical pages";
const std::string TraceConfig::FieldPageSize = "page size";
const std::string TraceConfig::FieldSetSize = "set size";
const std::string TraceConfig::FieldLineSize = "line size";
const std::string TraceConfig::FieldWriteThrough = "write through/no write allocate";
const std::string TraceConfig::FieldVirtualAddresses = "virtual addresses";
const std::string TraceConfig::FieldTLB = "tlb";

TraceConfig::TraceConfig(const std::string& configFileName)
{
  // Open the file for reading line by line
  std::ifstream configFile { configFileName.c_str() };
  if (!configFile)
  {
    throw std::runtime_error("Could not open the specified trace.config file");
  }

  // Parse the file into a map where the header and field names are keys
  std::map<std::string,std::map<std::string,std::string>> configMap{};
  std::string header = HeaderNoHeader;
  std::string line;
  while (std::getline(configFile, line))
  {
    if (line.empty())
    {
      header = HeaderNoHeader;
    }
    else if (!IsAField(line))
    {
      header = GetHeader(line);
    }
    else
    {
      auto key = GetFieldKey(line);
      auto val = GetFieldVal(line);
      configMap[header][key] = val;
    }
  }
  configFile.close();

  // Set the trace config properties using the map
  try
  {
    TLBEntries = std::stoi(configMap[HeaderDataTLBConfiguration][FieldNumberOfEntries]);
    PageTableVirtualPages = std::stoi(configMap[HeaderPageTableConfiguration][FieldNumberOfVirtualPages]);
    PageTablePhysicalPages = std::stoi(configMap[HeaderPageTableConfiguration][FieldNumberOfPhysicalPages]);
    PageTablePageSize = std::stoi(configMap[HeaderPageTableConfiguration][FieldPageSize]);
    DataCacheSets = std::stoi(configMap[HeaderDataCacheConfiguration][FieldNumberOfEntries]);
    DataCacheSetSize = std::stoi(configMap[HeaderDataCacheConfiguration][FieldSetSize]);
    DataCacheLineSize = std::stoi(configMap[HeaderDataCacheConfiguration][FieldLineSize]);
    DataCacheWriteThrough = configMap[HeaderDataCacheConfiguration][FieldWriteThrough] == FieldValueTrue; 
    UseVirtualAddreses = configMap[HeaderNoHeader][FieldVirtualAddresses] == FieldValueTrue;
    UseTLB = configMap[HeaderNoHeader][FieldTLB] == FieldValueTrue;
  }
  catch (std::exception& e)
  {
    throw std::runtime_error("Unable to parse the trace.config file");
  }

  // Validate properties parsed from trace config
  if (TLBEntries <= 0
      || (TLBEntries & (TLBEntries - 1))
      || TLBEntries > TLBMaxEntries)
  {
    throw std::invalid_argument("Invalid number of TLB entries specified");
  }
  if (PageTableVirtualPages <= 0
      || (PageTableVirtualPages & (PageTableVirtualPages - 1))
      || PageTableVirtualPages > PageTableMaxVirtualPages)
  {
    throw std::invalid_argument("Invalid number of Page Table virtual pages specified");
  }
  if (PageTablePhysicalPages <= 0
      || (PageTablePhysicalPages & (PageTablePhysicalPages - 1))
      || PageTablePhysicalPages > PageTableMaxPhysicalPages)
  {
    throw std::invalid_argument("Invalid number of Page Table physical pages specified");
  }
  if (PageTablePageSize <= 0
      || (PageTablePageSize & (PageTablePageSize - 1)))
  {
    throw std::invalid_argument("Invalid Page Table page size");
  }
  if (DataCacheSets <= 0
      || (DataCacheSets & (DataCacheSets - 1))
      || DataCacheSets > DataCacheMaxSets)
  {
    throw std::invalid_argument("Invalid number of Data Cache sets");
  }
  if (DataCacheSetSize <= 0
      || (DataCacheSetSize & (DataCacheSetSize - 1))
      || DataCacheSetSize > DataCacheMaxSetSize)
  {
    throw std::invalid_argument("Invalid Data Cache set size");
  }
  if (DataCacheLineSize <= DataCacheMinLineSize
      || (DataCacheLineSize & (DataCacheLineSize - 1)))
  {
    throw std::invalid_argument("Invalid Data Cache line size");
  }

  // Calculate the remaining trace config properties
  MainMemorySize = PageTablePhysicalPages * PageTablePageSize;
  BitsPageTableOffset = std::log2(PageTablePageSize);
  BitsPageTableIndex = std::log2(PageTableVirtualPages * PageTablePageSize) - BitsPageTableOffset;
  BitsDataCacheOffset = std::log2(DataCacheLineSize);
  BitsDataCacheIndex = std::log2(DataCacheSets * DataCacheSetSize);
  BitsDataCacheTag = std::log2(MainMemorySize) - BitsDataCacheIndex - BitsDataCacheOffset;
}

bool TraceConfig::IsAField(const std::string& line)
{
  return line.find(FieldDelimeter) != std::string::npos;
}

std::string TraceConfig::GetHeader(const std::string& line)
{
  return tolower_copy(trim_copy(line));
}

std::string TraceConfig::GetFieldKey(const std::string& line)
{
  auto keyEndPosition = line.find(FieldDelimeter);
  if (keyEndPosition == std::string::npos)
  {
    throw std::runtime_error("Unable to get trace.config file field key");
  }
  return tolower_copy(trim_copy(line.substr(0, keyEndPosition)));
}

std::string TraceConfig::GetFieldVal(const std::string& line)
{
  auto valStartPosition = line.find(FieldDelimeter) + 1;
  if (valStartPosition == std::string::npos)
  {
    throw std::runtime_error("Unable to get trace.config file field value");
  }
  return tolower_copy(trim_copy(line.substr(valStartPosition)));
}
