#ifndef BASE_COLLIDER_H_CLASS
#define BASE_COLLIDER_H_CLASS

#include <glm/glm.hpp>

namespace physics2D
{
	class BaseCollider
	{
	protected:
		BaseCollider(const glm::vec2 position) : position(position) {}

	public:
		virtual glm::vec2 findFurthestPoint(const glm::vec2& direction) const = 0;
	
	public:
		glm::vec2 position;

	};
}

#endif
