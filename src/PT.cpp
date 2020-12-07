#include "PT.h"
#include "IPageTable.h"
#include "TraceConfig.h"
#include "SwapSubject.h"
#include <cmath>
#include <iostream>

PT::PT(const TraceConfig& config, SwapSubject& swapSubject)
	: Config{ config }, Swap{ swapSubject }
{
	pageTable.reserve(Config.PageTablePhysicalPages);
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
	
	if (pageTable.size() == 0) 
	{
		diskReference++;
		pageTable.push_back(entry);
		returnType.PTHit = false;
		returnType.PhysicalAddress = entry.offset;
		return returnType;
	}
	
	returnType = checkPT(entry);
	
	if(returnType.PTHit)
		return returnType;
	
	if (pageTable.size() < Config.PageTablePhysicalPages)
	{
		pageTable.push_back(entry);
		returnType.PhysicalAddress = (((pageTable.size() - 1) * Config.PageTablePageSize) << Config.BitsPageTableOffset) + entry.offset;
		return returnType;
	}
	else
	{
		diskReference++;
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
			hits++;
			returnType.PTHit = true;
			returnType.PhysicalAddress = ((i * Config.PageTablePageSize) << Config.BitsPageTableOffset) + entry.offset;
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
	int replace;
	double current = pageTable[0].time;
	for (int i = 1; i < Config.PageTablePhysicalPages; i++)
	{
		if (pageTable[i].time < current)
		{
			current = pageTable[i].time;
			replace = i;
		}
	}
	
	returnType.PTHit = false;
	returnType.PhysicalAddress = ((replace * Config.PageTablePageSize) << Config.BitsPageTableOffset) + entry.offset;

	// Publish eviction for swap handler
	publishEvent(returnType.PhysicalAddress, pageTable[replace].virtualAddress);
	
	// entry placed at index
	pageTable.assign(replace, entry);
	return returnType;
}

void PT::publishEvent(int physicalAddress, int virtualAddress)
{
	SwapEvent se;
	se.PAEvictedFromMainMemory = physicalAddress;
	se.VAEvictedFromMainMemory = virtualAddress;
	PT::Swap.Publish(se);
}

int PT::GetHits()
{
	return hits;
}

int PT::GetFaults()
{
	return faults;
}

int PT::GetDiskReferences()
{
	return diskReference;
}