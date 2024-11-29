#ifndef PHYSICS_WORLD_HEADER
#define PHYSICS_WORLD_HEADER

#include <vector>
#include "PhysicsBody.h"
#include "Manifold.h"


namespace physics2D
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld() : m_gravity(0, -10)
		{ }

		PhysicsWorld(const glm::vec2 gravity) : m_gravity(gravity)
		{ }

		void addBody(const PhysicsBody& body);

		template<typename ...Args>
		void addBody(Args&&... args);

		void fixedUpdate(float deltaTime);

		void jump()
		{
			for (PhysicsBody& body : bodies)
			{
				body.velocity(glm::vec2(1 * (rand() % 3 - 1), 1 * (rand() % 3 - 1)));
			}
		}

		

	private:
		std::vector<PhysicsBody> bodies;
		glm::vec2 m_gravity;
	};
	
	template<typename ...Args>
	inline void PhysicsWorld::addBody(Args&&...args)
	{
		PhysicsBody newBody = PhysicsBody(std::forward<Args>(args)...);

		bodies.push_back(newBody);
	}


}

#endif 