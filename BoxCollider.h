#ifndef BOX_COLLIDER_H_CLASS
#define BOX_COLLIDER_H_CLASS

#include <limits>
#include "BaseCollider.h"

namespace physics2D
{
	class BoxCollider : public BaseCollider
	{
	public:
		BoxCollider(const glm::vec2 position, const float rotation, glm::vec2 scale);

		/// <summary>
		/// Find the furthest point on a rotated box within a given direction
		/// (Assumes the direction is normalized, may lead to unexpected results)
		/// </summary>
		/// <param name="direction">search direction</param>
		/// <returns>Furthest point</returns>
		glm::vec2 findFurthestPoint(const glm::vec2& direction) const override;

		glm::vec2 m_scale;
	private:
		const static size_t BOX_POINTS_SIZE = 4;
		constexpr static double DEG_TO_RED = 0.017453292519943295769236907684886f;
	};
}

#endif