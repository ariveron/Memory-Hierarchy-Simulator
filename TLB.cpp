#include "TLB.h"
#include "ITranslationBuffer.h"
#include "TraceConfig.h"
#include "IPageTable.h"
#include "SwapSubject.h"

#include <functional>
#include <vector>
#include <chrono>
#include <limits>

int numHits = 0;
int numMisses = 0;
int numPTRefs = 0;

struct tlbVectorEntry
{
    int virtualAddress;
    int physicalAddress;
    double lastTimeAccessed;
    bool isValid;
};

std::vector<tlbVectorEntry> tlbVector;

TLB::TLB(const TraceConfig& config, IPageTable& pt, SwapSubject& swapSubject)
        : Config { config }, PT { pt }
{
    // Subscribe to swap events
    swapSubject.Subscribe(GetSwapHandler());

    // TODO
    // Any additional intialization
    tlbVectorEntry emptyEntry{};
    emptyEntry.virtualAddress = 0;
    emptyEntry.physicalAddress = 0;
    emptyEntry.lastTimeAccessed = 0;
    emptyEntry.isValid = false;
    tlbVector = std::vector<tlbVectorEntry>(TraceConfig::TLBMaxEntries, emptyEntry);
}

std::function<void(SwapEvent)> TLB::GetSwapHandler()
{
    return [&](SwapEvent swapEvent) {
        // TODO
        // Gets called when a swap happens in the PT
        // Has access to all properties of this object
        // Do here whatever you need to do if a page got
        // evicted from main memory that the TLB was caching
        // You can see what page got evicted in the swapEvent


        //int address = swapEvent.PAEvictedFromMainMemory;
        //if(!entryPresent(swapEvent.PAEvictedFromMainMemory)) return;

        //EvictEntry(swapEvent.PAEvictedFromMainMemory);
    };
}

TLBReturnType TLB::GetPhysicalAddress(int virtualAddress)
{
    // TODO

    int i = 0;
    int addressFound = 0;
    TLBReturnType returnType{};
    while(addressFound == 0 && i < TraceConfig::TLBMaxEntries)
    {
        if(tlbVector[i].virtualAddress == virtualAddress)
        {
            addressFound = 1;
            numHits++;
            returnType.TLBHit = true;
            returnType.PhysicalAddress = tlbVector[i].physicalAddress;
            returnType.PTHit = false;
        }
    }
    if(addressFound == 0)
    {
        numMisses++;
        numPTRefs++;
        PTReturnType ptReturn = PT.GetPhysicalAddress(virtualAddress);
        returnType.PhysicalAddress = ptReturn.PhysicalAddress;
        returnType.PTHit = ptReturn.PTHit;
        returnType.TLBHit = false;

        //adding address to TLB since it was not in it
        if(Config.TLBEntries == TraceConfig::TLBMaxEntries)
        {
            int physicalAddress = tlbVector[0].physicalAddress;
            double oldestEntryTime = tlbVector[0].lastTimeAccessed;
            for(int k = 0; k < TraceConfig::TLBMaxEntries; k++)
            {
                if(tlbVector[k].lastTimeAccessed < oldestEntryTime)
                {
                    oldestEntryTime = tlbVector[k].lastTimeAccessed;
                    physicalAddress = tlbVector[k].physicalAddress;
                }
            }
            EvictEntry(physicalAddress);
        }
        tlbVectorEntry newEntry{};
        newEntry.physicalAddress = ptReturn.PhysicalAddress;
        newEntry.isValid = true;
        newEntry.virtualAddress = virtualAddress;
        newEntry.lastTimeAccessed = std::chrono::system_clock::now().time_since_epoch().count();
        AddEntry(newEntry);
    }
    return returnType;
}

void TLB::EvictEntry(int physicalAddress)
{
    int i = 0;
    int addressFound = 0;
    while(i < TraceConfig::TLBMaxEntries && addressFound == 0)
    {
        if(tlbVector[i].physicalAddress == physicalAddress)
        {
            addressFound = 1;
            tlbVector[i].isValid = false;
        }
        i++;
    }
}

void TLB::AddEntry(tlbVectorEntry newEntry)
{
    int i = 0;
    int locationFound = 0;
    while(i < TraceConfig::TLBMaxEntries && locationFound == 0)
    {
        if(!tlbVector[i].isValid)
        {
            locationFound = 1;
            tlbVector[i] = newEntry;
        }
    }
}

int TLB::GetHits()
{
    // TODO
    return numHits;
}

int TLB::GetMisses()
{
    // TODO
    return numMisses;
}

int TLB::GetPageTableReferences()
{
    // TODO
    return numPTRefs;
}

bool TLB::entryPresent(int physicalAddress) {
    int i = 0;
    bool entryFound = false;
    while(i < TraceConfig::TLBMaxEntries && !entryFound)
    {
        if(tlbVector[i].physicalAddress == physicalAddress)
        {
            entryFound = true;
        }
    }
    return entryFound;
}
