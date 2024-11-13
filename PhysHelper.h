#ifndef PHYS_HELPER_H
#define PHYS_HELPER_H

#include <glm/glm.hpp>
#include "BaseCollider.h"
namespace physics2D
{

	class PhysHelper
	{
	public:
		static glm::vec2 support(const BaseCollider& collA, const BaseCollider& collB,
			const glm::vec2& direction);

		static glm::vec2 getPerpendicular(const glm::vec2& realVec, const glm::vec2& dirVec);
		
		static glm::vec2 tripleProduct(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
	};

}

#endif