#ifndef PHYS_HELPER_H
#define PHYS_HELPER_H

#include <glm/glm.hpp>
#include "BaseCollider.h"

class PhysHelper
{
	static glm::vec2 support(const BaseCollider& collA, const BaseCollider& collB, const glm::vec2& direction)
	{
		return collA.findFurthestPoint(direction) - collB.findFurthestPoint(-direction);
	}

	static glm::vec2 tripleProduct(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
	{
		float dotAC = glm::dot(a, c);
		float dotBC = glm::dot(b, c);
		return { b.x * dotAC - a.x * dotBC, b.y * dotAC - a.y * dotBC };
	}
};
#endif