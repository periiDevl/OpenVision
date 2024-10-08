#ifndef GAME_OBJECT_CLASS_H
#define GAME_OBJECT_CLASS_H

#include <string>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:

	GameObject() : 
		isActive(true), transform(addComponent<Transform>())
	{ }
	GameObject(bool isActive) : 
		isActive(isActive), transform(addComponent<Transform>())
	{ }
	GameObject(std::string name) : 
		name(name), isActive(true), transform(addComponent<Transform>())
	{ }
	GameObject(std::string name, bool isActive) : 
		name(name), isActive(isActive), transform(addComponent<Transform>())
	{ }

	GameObject(GameObject&& value) noexcept = default;

	GameObject(const GameObject& value) = default;
	
	template<typename T, typename... Args>
	T& addComponent(Args&&... args)
	{
		auto component = std::make_unique<T>(std::forward<Args>(args)...);

		components[std::type_index(typeid(T))] = std::move(component);

		return *component;
	}
	template<typename T>
	T& addComponent(T& component) {
		
		auto uniquePtrComponent = std::make_unique<T>(component);
		
		components[std::type_index(typeid(T))] = std::move(uniquePtrComponent);
		
		return *uniquePtrComponent;
	}

	template<typename T>
	T& getComponent()
	{
		if (components.find(std::typeindex(typeid(T))) != components.end())
		{
			return static_cast<T*>(components[std::typeindex(typeid(T))].get());
		}

		throw std::runtime_error("Component was not found.");
	}

	template<typename T>
	void deleteComponent()
	{
		if (components.find(std::type_index(typeid(T))) == components.end())
		{
			throw std::runtime_error("Couldn't remove component (Didn't find it)");
		}

		components.erase(std::type_index(typeid(T)));
	}

	void setTag(std::string newTag) { tag = newTag; }
	const std::string& getTag() { return tag; }

	void setName(std::string newName) { name = newName; }
	const std::string& getName() { return name; }

	void setActive(bool active) { isActive = active; }
	const bool& getActive() { return isActive; }

	Transform& transform;

private:
	bool isActive;

	std::string tag;

	std::string name;
	
	std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};

#endif