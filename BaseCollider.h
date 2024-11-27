#ifndef BASE_COLLIDER_H_CLASS
#define BASE_COLLIDER_H_CLASS

#include <glm/glm.hpp>

namespace physics2D
{
	class BaseCollider
	{
	public:

		BaseCollider(const glm::vec2 position, const float rotation) 
			: m_position(position), m_rotation(rotation) {}

		virtual glm::vec2 findFurthestPoint(const glm::vec2& direction) const = 0;
		
	public:
		glm::vec2 m_position;
		float m_rotation;
	};
}

#endif
