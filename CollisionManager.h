#ifndef COLLISION_MANAGER_HEADER
#define COLLISION_MANAGER_HEADER

#include "PhysicsBody.h"
#include "Manifold.h"
#include "Simplex.h"
#include "GJK.h"
#include "EPA.h"

namespace physics2D
{
	class CollisionManager
	{
	public:
		static bool isCollide(const PhysicsBody& body1, const PhysicsBody& body2, Manifold& manifold);

		static void resolveManifold(Manifold& manifold);
	};
}
#endif