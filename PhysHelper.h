#ifndef PHYS_HELPER_H
#define PHYS_HELPER_H

#include <glm/glm.hpp>
#include "BaseCollider.h"

class PhysHelper
{
public:
	static glm::vec2 support(const BaseCollider& collA, const BaseCollider& collB, 
		const glm::vec2& direction)
	{
		return collA.findFurthestPoint(direction) - collB.findFurthestPoint(-direction);
	}

	static glm::vec2 tripleProduct(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
	{
		glm::vec3 a3D = { a.x, a.y, 0.0f };
		glm::vec3 b3D = { b.x, b.y, 0.0f };
		glm::vec3 c3D = { c.x, c.y, 0.0f };

		// Perform the first cross product: b3D x c3D
		glm::vec3 cross1 = glm::cross(b3D, c3D);

		// Perform the second cross product: cross1 x a3D
		glm::vec3 cross2 = glm::cross(cross1, a3D);

		// Project the result back to 2D (x and y components only)
		return glm::vec2(cross2.x, cross2.y);
	}
};
#endif