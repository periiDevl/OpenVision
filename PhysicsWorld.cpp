#include "PhysicsWorld.h"
#include "CollisionManager.h"

using namespace physics2D;

void PhysicsWorld::addBody(const PhysicsBody& body)
{
	bodies.push_back(body);
}

void PhysicsWorld::fixedUpdate(float deltaTime)
{
	std::vector<Manifold> manifolds;
	for (size_t i = 0; i < bodies.size(); i++)
	{
		if (bodies[i].isStatic())
			continue;

		bodies[i].applyForce(m_gravity * bodies[i].mass());
		bodies[i].fixedUpdate(deltaTime);
	}

	manifolds.clear();
	Manifold curManifold;

	for (size_t i = 0; i < bodies.size()-1; i++)
	{
		for (size_t j = i+1; j < bodies.size(); j++)
		{
			if (!CollisionManager::isCollide(bodies[i], bodies[j], curManifold))
			{
				continue;
			}

			curManifold.body1 = &bodies[i];
			curManifold.body2 = &bodies[j];

			manifolds.push_back(curManifold);
		}
	}

	for (Manifold& manifold : manifolds)
	{
		manifold.body1->getCollider().m_position -= manifold.mtv * 0.5f;
		manifold.body2->getCollider().m_position += manifold.mtv * 0.5f;
	}
}
