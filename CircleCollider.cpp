#include "CircleCollider.h"

using namespace physics2D;

CircleCollider::CircleCollider(const glm::vec2 position, const float rotation, float radius) 
	: BaseCollider(position, rotation), m_radius(radius)
{ }

glm::vec2 CircleCollider::findFurthestPoint(const glm::vec2& direction) const
{
	return m_position + direction * m_radius;
}
