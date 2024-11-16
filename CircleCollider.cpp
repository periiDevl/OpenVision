#include "CircleCollider.h"

using namespace physics2D;

CircleCollider::CircleCollider(const glm::vec2 position, float radius) : BaseCollider(position), radius(radius)
{ }

glm::vec2 CircleCollider::findFurthestPoint(const glm::vec2& direction) const
{
	return position + direction * radius;
}
