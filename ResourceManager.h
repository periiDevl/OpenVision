#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H

#include <unordered_map>
#include <typeindex>

class ResourceManager
{
public:
	template<typename T>
	static T& getResource()
	{
		static_assert(!resources.find(std::type_index(typeid(T))));

		return static_cast<T&>(resources[typeid(T)]);
	}

	template<typename T>
	static void registerResource(T& value)
	{
		resources[std::type_index(typeid(T))] = &value;
	}

	template<typename T>
	static void registerResource(T* value)
	{
		resources[std::type_index(typeid(T))] = value;
	}

	// need to add destroy resource and make sure they are the owners of the resource in question

private:
	static std::unordered_map<std::type_index, void*> resources;
};


#endif
