#ifndef CIRCLE_COLLIDER_H_CLASS
#define CIRCLE_COLLIDER_H_CLASS

#include "BaseCollider.h"

namespace physics2D
{
	class CircleCollider : public BaseCollider
	{
	public:
		CircleCollider(const glm::vec2 position, const float rotation, float radius);

		/// <summary>
		/// Find the furthest point on a circle within a given direction
		/// (Assumes the direction is normalized, may lead to unexpected results)
		/// </summary>
		/// <param name="direction">search direction</param>
		/// <returns>Furthest point</returns>
		glm::vec2 findFurthestPoint(const glm::vec2& direction) const override;
	
	private:
		float m_radius;
	};
}

#endif