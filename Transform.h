#ifndef TRANSFORM_CLASS_H
#define TRANSFORM_CLASS_H

#include <glm/glm.hpp>
#include "Component.h"

class GameObject;

class Transform : public Component
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

	Transform(GameObject& owner, glm::vec2 position, float rotation, glm::vec2 scale,
		glm::vec2 localPosition, float localRotation, glm::vec2 localScale) :
		Component(owner), position(position), rotation(rotation), scale(scale),
		localPosition(localPosition), localRotation(localRotation), localScale(localScale)
	{ }

	Transform(GameObject& owner, glm::vec2 position, float rotation, glm::vec2 scale)
		: Component(owner), position(position), rotation(rotation), scale(scale),
		localPosition(0.0f, 0.0f), localRotation(0.0f), localScale(1.0f, 1.0f)
	{ }


	void setParent(Transform& newParent)
	{
		// Can't set a parent to itself
		if (&newParent == this)
		{
			return;
		}

		parent = &newParent;
		
		localPosition = parent->position - position;
		localRotation = parent->rotation - rotation;
		parent->addChild(this);
	}
	void setParent(Transform* newParent)
	{
		// Can't set a parent to itself
		if (newParent == this)
		{
			return;
		}

		parent = newParent;

		if (parent)
		{ 
			localPosition = parent->position - position;
			localRotation = parent->rotation - rotation;
			parent->addChild(this);
		}
	}

	void addChild(Transform& child)
	{
		if (&child != this)
		{
			children.push_back(&child);
			child.setParent(this);

			child.localPosition = position - child.position;
			child.localRotation = rotation - child.rotation;
		}
	}
	void addChild(Transform* child)
	{
		if (child && child != this)
		{
			children.push_back(child);
			child->setParent(this);

			child->localPosition = position - child->position;
			child->localRotation = rotation - child->rotation;
		}
	}

	Transform* getParent()
	{
		return parent;
	}

	Transform* getChild(int index)
	{
		if (index < children.size())
		{
			return children[index];
		}
		return nullptr;
	}

	glm::vec2 position;
	float rotation;
	glm::vec2 scale;

	glm::vec2 localPosition;
	float localRotation;
	glm::vec2 localScale; 

private:
	Transform* parent;
	std::vector<Transform*> children;

};

#endif