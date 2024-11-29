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
	manifolds.clear();
	Manifold curManifold;

	for (size_t i = 0; i < bodies.size()-1; i++)
	{
		for (size_t j = i+1; j < bodies.size(); j++)
		{
			if (bodies[i].isStatic() && bodies[j].isStatic())
			{
				continue;
			}

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
		PhysicsBody& body1 = *manifold.body1;
		PhysicsBody& body2 = *manifold.body2;
		if (body1.isStatic())
		{
			body2.getCollider().m_position -= manifold.mtv;
		}
		else if (body2.isStatic())
		{
			body1.getCollider().m_position += manifold.mtv;
		}
		else
		{
			body1.getCollider().m_position += manifold.mtv * 0.5f;
			body2.getCollider().m_position -= manifold.mtv * 0.5f;
		}

		CollisionManager::resolveManifold(manifold);
	}

	for (size_t i = 0; i < bodies.size(); i++)
	{
		if (bodies[i].isStatic())
			continue;

		bodies[i].applyForce(m_gravity * bodies[i].mass());
		bodies[i].fixedUpdate(deltaTime);
	}

}
