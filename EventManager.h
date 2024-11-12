#ifndef EVENT_MANAGER_HEADER
#define EVENT_MANAGER_HEADER

#include <functional>
#include <any>
#include <unordered_map>
#include <typeindex>

class EventManager
{
public:
	template<typename Event>
	using EventCallback = std::function<void(const Event*)>;

	static void initialize();

	static void clearCallbacks();

	template<typename Event>
	static void callCallback(const Event& eventData);

	template<typename Event>
	static void addCallback(const EventCallback<Event>& callback);
	
	template<typename Event>
	static void addEvent();

	template<typename Event>
	static void removeEvent();

private:
	EventManager() {}

	using GeneralEventCallback = std::function<void(const void*)>;

	std::unordered_map<std::type_index, std::vector<GeneralEventCallback>> events;

	static EventManager singleton;
};

template<typename Event>
inline void EventManager::callCallback(const Event& eventData)
{
	auto it = singleton.events.find(std::type_index(typeid(Event)));
	
	if (it == singleton.events.end())
	{
		return;
	}

	for (auto& callback : it->second)
	{
		callback(&eventData);
	}

}

template<typename Event>
inline void EventManager::addCallback(const EventCallback<Event>& callback)
{
	std::type_index type = std::type_index(typeid(Event));

	GeneralEventCallback generalCallback = [callback](const void* event) {
		callback(static_cast<const Event*>(event));
		};

	singleton.events[type].push_back(generalCallback);
}

template<typename Event>
inline void EventManager::addEvent()
{
	singleton.events[std::type_index(typeid(Event))] = std::vector<GeneralEventCallback>();
}

template<typename Event>
inline void EventManager::removeEvent()
{
	singleton.events.erase(std::type_index(typeid(Event)));
}

#endif
