#include "TLB.h"
#include "ITranslationBuffer.h"
#include "TraceConfig.h"
#include "IPageTable.h"
#include "SwapSubject.h"

#include <functional>
#include <vector>
#include <chrono>
#include <limits>

#include <iostream>

std::vector<tlbVectorEntry> tlbVector;

TLB::TLB(const TraceConfig& config, IPageTable& pt, SwapSubject& swapSubject)
        : Config { config }, PT { pt }, numHits { 0 }, numMisses { 0 }, numPTRefs { 0 }, numValidEntries { 0 }
{
    // Subscribe to swap events
    swapSubject.Subscribe(GetSwapHandler());

    // Any additional intialization
    tlbVectorEntry emptyEntry{};
    emptyEntry.physicalPageNumber = -1;
    emptyEntry.virtualPageNumber = -1;
    emptyEntry.lastTimeAccessed = 0;
    emptyEntry.isValid = false;
    tlbVector = std::vector<tlbVectorEntry>(Config.TLBEntries, emptyEntry);
}

std::function<void(SwapEvent)> TLB::GetSwapHandler()
{
    return [&](SwapEvent swapEvent) {
        int address = swapEvent.PAEvictedFromMainMemory;
        if(!entryPresent(swapEvent.PAEvictedFromMainMemory)) return;

        EvictEntry(swapEvent.PAEvictedFromMainMemory);
    };
}

TLBReturnType TLB::GetPhysicalAddress(int virtualAddress, bool isWrite)
{
    int virtualPageNumber = virtualAddress >> Config.BitsPageTableOffset;

    int i = 0;
    int addressFound = 0;
    TLBReturnType returnType{};
    while(addressFound == 0 && i < Config.TLBEntries)
    {
        if(tlbVector[i].virtualPageNumber == virtualPageNumber)
        {
            addressFound = 1;
            numHits++;
            returnType.TLBHit = true;
            returnType.PhysicalAddress = CalculatePhysicalAddress(tlbVector[i].physicalPageNumber, virtualAddress);
            returnType.PTHit = false;
        }
        i++;
    }

    if(addressFound == 0)
    {
        numMisses++;
        numPTRefs++;
        PTReturnType ptReturn = PT.GetPhysicalAddress(virtualAddress, isWrite);
        returnType.PhysicalAddress = ptReturn.PhysicalAddress;
        returnType.PTHit = ptReturn.PTHit;
        returnType.TLBHit = false;

        //adding address to TLB since it was not in it
        if(numValidEntries == Config.TLBEntries)
        {
            int physicalPageNumber = tlbVector[0].physicalPageNumber;
            double oldestEntryTime = tlbVector[0].lastTimeAccessed;
            for(int k = 0; k < Config.TLBEntries; k++)
            {
                if(tlbVector[k].lastTimeAccessed < oldestEntryTime)
                {
                    oldestEntryTime = tlbVector[k].lastTimeAccessed;
                    physicalPageNumber = tlbVector[k].physicalPageNumber;
                }
            }
            EvictEntry(physicalPageNumber);
        }
        tlbVectorEntry newEntry{};
        newEntry.physicalPageNumber = ptReturn.PhysicalAddress >> Config.BitsPageTableOffset;
        newEntry.isValid = true;
        newEntry.virtualPageNumber = virtualPageNumber;
        newEntry.lastTimeAccessed = std::chrono::system_clock::now().time_since_epoch().count();
        AddEntry(newEntry);
    }
    return returnType;
}

void TLB::EvictEntry(int physicalPageNumber)
{
    int i = 0;
    int pageNumberFound = 0;
    while(i < Config.TLBEntries && pageNumberFound == 0)
    {
        if(tlbVector[i].physicalPageNumber == physicalPageNumber)
        {
            pageNumberFound = 1;
            tlbVector[i].isValid = false;
        }
        i++;
    }
    numValidEntries--;
}

void TLB::AddEntry(tlbVectorEntry newEntry)
{
    int i = 0;
    int locationFound = 0;
    while(i < Config.TLBEntries && locationFound == 0)
    {
        if(!tlbVector[i].isValid)
        {
            locationFound = 1;
            tlbVector[i] = newEntry;
        }
        i++;
    }
    numValidEntries++;
}

int TLB::GetHits()
{
    return numHits;
}

int TLB::GetMisses()
{
    return numMisses;
}

int TLB::GetPageTableReferences()
{
    return numPTRefs;
}

bool TLB::entryPresent(int physicalAddress) {
    int i = 0;
    bool entryFound = false;
    while(i < Config.TLBEntries && !entryFound)
    {
        if(tlbVector[i].physicalPageNumber == (physicalAddress >> Config.BitsPageTableOffset))
        {
            entryFound = true;
        }
        i++;
    }
    return entryFound;
}

int TLB::CalculatePhysicalAddress(int physicalPageNumber, int virtualAddress)
{
    // Combine physical page number bits with virtual page offset bits to get physical address
    return (physicalPageNumber << Config.BitsPageTableOffset) | (virtualAddress & ((1 << Config.BitsPageTableOffset) - 1));
}
