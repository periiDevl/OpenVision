#ifndef COLLISION_MANAGER_HEADER
#define COLLISION_MANAGER_HEADER

#include "Manifold.h"
#include "PhysicsBody.h"

namespace physics2D
{
	class CollisionManager
	{
	public:
		static bool isCollide(const PhysicsBody& body1, const PhysicsBody& body2, Manifold& manifold);
	};
}
#endif