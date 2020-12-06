#include "SwapSubject.h"

#include <functional>
#include <list>

SwapSubject::SwapSubject()
  : SwapEventHandlers {}
{}

void SwapSubject::Subscribe(std::function<void(SwapEvent)> swapEventHandler)
{
  SwapEventHandlers.push_back(swapEventHandler);
}

void SwapSubject::Publish(SwapEvent swapEvent)
{
  for (auto handler = SwapEventHandlers.begin(); handler != SwapEventHandlers.end(); handler++)
  {
    (*handler)(swapEvent);
  }
}
