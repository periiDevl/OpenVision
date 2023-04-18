#include "CircleCollider.h"

CircleCollider::CircleCollider(vec2* pos, float* rot, vec2* sca)
{
	float maxScale = std::max(sca->x, sca->y);
	radius = maxScale;
	
	bMax = vec2(radius, radius);
	bMin = vec2(-radius, -radius);
	bRadius = radius;
}

vec2 CircleCollider::GetSupportPoint(vec2 direction) {
	return *Position + direction * radius;
}

void CircleCollider::CalculateAABB()
{
	bMax = vec2( radius, radius);
	bMin = vec2(-radius,-radius);
}

void CircleCollider::CalculateBoundRadius() {
	bRadius = radius;
}
