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

	static glm::vec2 getPerpendicular(const glm::vec2& realVec, const glm::vec2& dirVec)
	{
		glm::vec2 result = { -realVec.y, realVec.x };

		if (glm::dot(result, dirVec) < 0)
		{
			result = -result;
		}

		return result;
	}
	static glm::vec2 tripleProduct(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
	{
		glm::vec3 a3D = { a.x, a.y, 0.0f };
		glm::vec3 b3D = { b.x, b.y, 0.0f };
		glm::vec3 c3D = { c.x, c.y, 0.0f };

		glm::vec3 cross1 = glm::cross(a3D, b3D);

		glm::vec3 cross2 = glm::cross(cross1, c3D);

		return glm::vec2(cross2.x, cross2.y);
	}
};
#endif