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
		int virtualAddress;
		int virtualPage;
		int offset;
		double time;
	};
	std::vector<PTE> pageTable;
	PT(const TraceConfig& config, SwapSubject& swapSubject);
	
	// Methods from interface
	PTReturnType GetPhysicalAddress(int virtualAddress, bool isWrite) override;
	int GetHits() override;
	int GetFaults() override;
	int GetDiskReferences() override;
	void SetDirtyFlag(int physicalAddress) override;

private:
	int DiskReference;
	int Hits;
	int Faults;
	const TraceConfig& Config;
	SwapSubject& Swap;
	PTReturnType checkPT(PTE entry);
	PTReturnType evictPage(PTE entry);
	void publishEvent(int physicalAddress, int virtualAddress);
};

#endif
