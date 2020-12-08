#include "PT.h"
#include "IPageTable.h"
#include "TraceConfig.h"
#include "SwapSubject.h"
#include <cmath>
#include <iostream>

PT::PT(const TraceConfig& config, SwapSubject& swapSubject)
	: Config{ config }, Swap{ swapSubject }, Hits{ 0 }, DiskReference{ 0 }, Faults { 0 }
{
	auto pageTable = std::vector<PTE>(Config.PageTablePhysicalPages);
}

PTReturnType PT::GetPhysicalAddress(int virtualAddress, bool isWrite)
{
	PTReturnType returnType;
	PTE entry;
	entry.dirty = isWrite;
	entry.virtualAddress = virtualAddress;
	entry.virtualPage = virtualAddress >> Config.BitsPageTableOffset;
	entry.offset = virtualAddress & ((2 << (Config.BitsPageTableOffset - 1)) - 1);
	entry.time = std::chrono::system_clock::now().time_since_epoch().count();
	
	if (pageTable.empty()) 
	{
		DiskReference++;
		Faults++;

		pageTable.push_back(entry);
		returnType.PTHit = false;
		returnType.PhysicalAddress = entry.offset;
		return returnType;
	}
	
	returnType = checkPT(entry);
	
	if(returnType.PTHit)
		return returnType;

	DiskReference++;
	Faults++;
	
	if (pageTable.size() < Config.PageTablePhysicalPages)
	{
		pageTable.push_back(entry);
		returnType.PhysicalAddress = ((pageTable.size() - 1) * Config.PageTablePageSize) | entry.offset;
		return returnType;
	}
	else
	{
		returnType = evictPage(entry);
		return returnType;
	}
}

PTReturnType PT::checkPT(PTE entry)
{	
	PTReturnType returnType;
	for (int i = 0; i < pageTable.size(); i++)
	{
		if (pageTable[i].virtualPage == entry.virtualPage)
		{   
			Hits++;
			returnType.PTHit = true;
			returnType.PhysicalAddress = (i * Config.PageTablePageSize) | entry.offset;
			pageTable[i].time = std::chrono::system_clock::now().time_since_epoch().count();
			return returnType;
			break;
		}
	}
	returnType.PTHit = false;
	return returnType;
}

PTReturnType PT::evictPage(PTE entry) 
{
	PTReturnType returnType;
	auto replace = 0;
	auto current = std::numeric_limits<double>::max();
	for (int i = 1; i < Config.PageTablePhysicalPages; i++)
	{
		if (pageTable[i].time < current)
		{
			current = pageTable[i].time;
			replace = i;
		}
	}
	
	// Check for a write
	if (pageTable[replace].dirty)
		DiskReference++;
	pageTable.assign(replace, entry);
	
	returnType.PTHit = false;
	returnType.PhysicalAddress = (replace * Config.PageTablePageSize) | entry.offset;
	return returnType;
}

void PT::publishEvent(int physicalAddress, int virtualAddress)
{
	SwapEvent se;
	se.PAEvictedFromMainMemory = physicalAddress;
	se.VAEvictedFromMainMemory = virtualAddress;
	PT::Swap.Publish(se);
}

void PT::SetDirtyFlag(int physicalAddress)
{
	auto pageIndex = physicalAddress >> Config.BitsPageTableOffset;
	pageTable[pageIndex].dirty = true;
}

int PT::GetHits()
{
	return Hits;
}

int PT::GetFaults()
{
	return Faults;
}

int PT::GetDiskReferences()
{
	return DiskReference;
}
