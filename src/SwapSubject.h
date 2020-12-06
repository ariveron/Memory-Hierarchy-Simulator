#ifndef SWAP_SUBJECT_H
#define SWAP_SUBJECT_H

#include <functional>
#include <list>

struct SwapEvent
{
  int VAEvictedFromMainMemory;
  int PAEvictedFromMainMemory;
};

class SwapSubject
{
public:
  SwapSubject();
  void Subscribe(std::function<void(SwapEvent)> swapEventHandler);
  void Publish(SwapEvent swapEvent);

private:
  std::list<std::function<void(SwapEvent)>> SwapEventHandlers;
};

#endif
