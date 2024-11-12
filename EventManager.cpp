#include "EventManager.h"
EventManager EventManager::singleton;

void EventManager::initialize()
{
	singleton = EventManager();
}

void EventManager::clearCallbacks()
{
	singleton.events.clear();
}



