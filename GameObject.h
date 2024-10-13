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

	GameObject() : isActive(true)
	{
		transform = addComponent<Transform>();
	}
	GameObject(bool isActive) : isActive(isActive)
	{
		transform = addComponent<Transform>();
	}
	GameObject(std::string name) :
		name(name), isActive(true)
	{
		transform = addComponent<Transform>();
	}
	GameObject(std::string name, bool isActive) :
		name(name), isActive(isActive)
	{
		transform = addComponent<Transform>();
	}

	GameObject(GameObject&& value) noexcept = default;

	GameObject(const GameObject& value) = default;

	template<typename T, typename... Args>
	T* addComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		std::unique_ptr<T> component = std::make_unique<T>(*this, std::forward<Args>(args)...);

		std::type_index type = std::type_index(typeid(T));
		components[type] = std::move(component);

		return static_cast<T*>(components[type].get());
	}

	template<typename T>
	T* addComponent(T& component) {

		auto uniquePtrComponent = std::make_unique<T>(component);

		components[std::type_index(typeid(T))] = std::move(uniquePtrComponent);

		return uniquePtrComponent;
	}

	template<typename T>
	bool hasComponent()
	{
		return components.find(std::type_index(typeid(T))) != components.end();
	}

	template<typename T>
	T& getComponent()
	{
		if (components.find(std::type_index(typeid(T))) != components.end())
		{
			T* ptr = dynamic_cast<T*>(components[std::type_index(typeid(T))].get());
			
			if (!ptr)
			{
				throw std::runtime_error("Component was found null.");
			}
		
			return *ptr;
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

public:
	Transform* transform;

private:
	bool isActive;

	std::string tag;

	std::string name;

	std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};

#endif