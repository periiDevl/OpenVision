#include "CircleCollider.h"

CircleCollider::CircleCollider(vec2* pos, float* rot, vec2* sca)
{
	Position = pos;
	Rotation = rot;
	Scale = sca;

	float maxScale = std::max(sca->x, sca->y);
	radius = maxScale / 2.0f;
	
	bMax = vec2(radius, radius);
	bMin = vec2(-radius, -radius);
	bRadius = radius;

	type = ColliderType::Circle;
}

vec2 CircleCollider::GetSupportPoint(vec2 direction) {
	return *Position + glm::normalize(direction) * radius;
}

void CircleCollider::CalculateAABB()
{
	bMax = vec2( radius, radius);
	bMin = vec2(-radius,-radius);
}

void CircleCollider::CalculateBoundRadius() {
	bRadius = radius;
}
