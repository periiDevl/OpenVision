#ifndef TRANSFORM_CLASS_H
#define TRANSFORM_CLASS_H

#include <glm/glm.hpp>
#include "Component.h"

class GameObject;

class Transform : public Component, public std::enable_shared_from_this<Transform>
{
public:

	Transform(GameObject& owner)
		: Component(owner), position(0.0f), rotation(0.0f), scale(1.0f),
		localPosition(0.0f), localRotation(0.0f), localScale(1.0f)
	{ }

	Transform(Transform&& other) noexcept
		: Component(other.gameObject), position(other.position), rotation(other.rotation),
		scale(other.scale), localPosition(other.localPosition),
		localRotation(other.localRotation), localScale(other.localScale)
	{ }

	Transform(GameObject& owner, glm::vec2 position, float rotation, glm::vec2 scale, std::weak_ptr<Transform> defaultParent) :
		Component(owner), position(position), rotation(rotation), scale(scale),
		localPosition(localPosition), localRotation(localRotation), localScale(localScale)
	{ 
		setParent(*defaultParent.lock());
	}

	Transform(GameObject& owner, glm::vec2 position, float rotation, glm::vec2 scale)
		: Component(owner), position(position), rotation(rotation), scale(scale),
		localPosition(0.0f, 0.0f), localRotation(0.0f), localScale(1.0f, 1.0f)
	{ }


	void setParent(Transform& newParent)
	{
		// Prevent self parent or redundant func
		if (&newParent == this || parent.lock().get() == &newParent) return;

		parent = newParent.shared_from_this();
		localPosition = position - newParent.position;
		localRotation = rotation - newParent.rotation;
	}
	void addChild(Transform& child) {
		// Prevent self-parenting or redundant setting
		if (&child == this || &child.getParent() == this) return;

		children.push_back(child.shared_from_this()); // Add the child
	}

	Transform& getParent() const {
		auto parentPtr = parent.lock();

		if (!parentPtr) {
			throw std::runtime_error("Parent transform is not set or has expired.");
		}

		return *parentPtr;
	}

	// Get a specific child transform by index
	Transform& getChild(int index) const {
		if (index < 0 || index >= children.size()) {
			throw std::runtime_error("Invalid child index.");
		}

		auto childPtr = children[index].get();
		if (!childPtr) {
			throw std::runtime_error("Child transform has expired.");
		}
		return *childPtr;
	}

	void move(const glm::vec2& deltaPosition)
	{
		position += deltaPosition;
		localPosition += deltaPosition;
	}
	void rotate(const float& angle)
	{
		rotation += angle;
		localRotation += angle;
	}

public:
	glm::vec2 position;
	float rotation;
	glm::vec2 scale;

	glm::vec2 localPosition;
	float localRotation;
	glm::vec2 localScale;

private:
	std::weak_ptr<Transform> parent;
	std::vector<std::shared_ptr<Transform>> children;

};

#endif