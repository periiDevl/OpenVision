#ifndef PHYSICS_BODY_HEADER
#define PHYSICS_BODY_HEADER

#include <glm/glm.hpp>
#include "BaseCollider.h"

namespace physics2D
{
	class PhysicsBody
	{
	public:
		PhysicsBody() = default;
		PhysicsBody(BaseCollider& collider) : m_collider(collider),
			m_force(0), m_acceleration(0), m_velocity(0), m_mass(2.0), m_invMass(1/2.0)
		{ }

		PhysicsBody(BaseCollider& collider, float mass) : m_collider(collider),
			m_force(0), m_acceleration(0), m_velocity(0), m_mass(mass), m_invMass(1 / mass)
		{ }
		
		void fixedUpdate(float deltaTime)
		{
			// apply acceleration, velocity and change to position
			m_acceleration = m_force * m_invMass; // a = F / m
			m_velocity += m_acceleration * deltaTime; // v = v + a * dt
			m_collider.m_position += m_velocity * deltaTime; // x = x + v * dt
			
			// reset force
			m_force -= m_force;
		}

		void mass(float val)
		{
			m_mass = val;
			m_invMass = 1 / val;
		}
		float mass()
		{
			return m_mass;
		}
		glm::vec2 velocity()
		{
			return m_velocity;
		}
		void velocity(const glm::vec2 val)
		{
			m_velocity = val;
		}

	private:
		BaseCollider& m_collider;
	
		float m_mass = 2;
		float m_invMass = 2;

		glm::vec2 m_velocity;
		glm::vec2 m_acceleration;

		glm::vec2 m_force;
	};
}

#endif