#ifndef PT_H
#define PT_H

#include "IPageTable.h"
#include "TraceConfig.h"
#include "SwapSubject.h"
#include <vector>
#include <chrono>
class PT : public IPageTable
{
public:
	struct PTE 
	{
		bool valid;
		bool dirty;
		long long virtualAddress;
		long long virtualPage;
		long long offset;
		double time;
	};
	std::vector<PTE> pageTable;
	PT(const TraceConfig& config, SwapSubject& swapSubject);
	
	// Methods from interface
	PTReturnType GetPhysicalAddress(long long virtualAddress, bool isWrite) override;
	long long GetHits() override;
	long long GetFaults() override;
	long long GetDiskReferences() override;
	void SetDirtyFlag(long long physicalAddress) override;

private:
	long long DiskReference;
	long long Hits;
	long long Faults;
	const TraceConfig& Config;
	SwapSubject& Swap;
	PTReturnType checkPT(PTE entry);
	PTReturnType evictPage(PTE entry);
	void publishEvent(long long physicalAddress, long long virtualAddress);
};

#endif
