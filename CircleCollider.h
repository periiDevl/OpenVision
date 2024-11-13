#ifndef CIRCLE_COLLIDER_H_CLASS
#define CIRCLE_COLLIDER_H_CLASS

#include "BaseCollider.h"

namespace physics2D
{
	class CircleCollider : public BaseCollider
	{
	public:
		CircleCollider(const glm::vec2 position, float radius);

		glm::vec2 findFurthestPoint(const glm::vec2& direction) const override;
	
	private:
		float radius;
	};
}

#endif