#ifndef CIRCLE_COLLIDER_H_CLASS
#define CIRCLE_COLLIDER_H_CLASS

#include "BaseCollider.h"

class CircleCollider : public BaseCollider
{
public:
	CircleCollider(glm::vec2 position, float radius) : radius(radius)
	{
		this->position = position;
	}

	float radius;

	glm::vec2 findFurthestPoint(const glm::vec2& direction) const override
	{
		return position + direction * radius;
	}
};

#endif