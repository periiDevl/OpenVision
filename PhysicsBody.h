#ifndef PHYSICS_BODY_HEADER
#define PHYSICS_BODY_HEADER

#include <glm/glm.hpp>
#include "BaseCollider.h"
#include<iostream>
#include <glm/gtx/string_cast.hpp>

namespace physics2D
{
	class PhysicsBody
	{
	public:
		PhysicsBody() = default;
		PhysicsBody(BaseCollider& collider) : m_collider(collider),
			m_force(0), m_acceleration(0), m_velocity(0), m_mass(2.0), m_invMass(1/2.0), m_isStatic(false)
		{ 
			this->mass(m_mass); // set invmass automatically with is static
		}

		PhysicsBody(BaseCollider& collider, float mass) : m_collider(collider),
			m_force(0), m_acceleration(0), m_velocity(0), m_mass(mass), m_invMass(1 / mass), m_isStatic(false)
		{
			this->mass(m_mass); // set invmass automatically with is static
		}

		PhysicsBody(BaseCollider& collider, float mass, bool isStatic) : m_collider(collider),
			m_force(0), m_acceleration(0), m_velocity(0), m_mass(mass), m_invMass(1 / mass), m_isStatic(isStatic)
		{ 
			this->mass(m_mass); // set invmass automatically with is static
		}
		
		void fixedUpdate(float deltaTime)
		{
			// apply acceleration, velocity and change to position
			m_acceleration = m_force * m_invMass; // a = F / m
			
			if (m_force != glm::vec2(0, -1))
			{
				std::cout << glm::to_string(m_velocity) << std::endl;
				m_velocity += m_acceleration * deltaTime; // v = v + a * dt
				std::cout << glm::to_string(m_velocity) << std::endl;
			}
			else
			{ 
				m_velocity += m_acceleration * deltaTime; // v = v + a * dt
			}

			m_collider.m_position += m_velocity * deltaTime; // x = x + v * dt
			

			// reset force
			m_force = glm::vec2(0.0f);

		}

		void mass(float val)
		{
			m_mass = (val <= 0.0f) ? 0.0f : val;
			m_invMass = (m_mass == 0.0f) ? 0.0f : 1.0f / m_mass;
		}
		float mass() const 
		{ 
			return m_mass; 
		}
		float invMass() const 
		{
			return m_invMass; 
		}


		glm::vec2 velocity()
		{
			if (isStatic())
				return glm::vec2(0);

			return m_velocity;
		}
		void velocity(const glm::vec2 val)
		{
			m_velocity = val;
		}

		void applyForce(const glm::vec2 force)
		{
			m_force += force;
		}

		bool isStatic() const
		{
			return m_isStatic;
		}
		void isStatic(bool val)
		{
			m_isStatic = val;
		}

		BaseCollider& getCollider() const
		{
			return m_collider;
		}

	private:
		BaseCollider& m_collider;
	
		bool m_isStatic = false;

		float m_mass = 2;
		float m_invMass = 2;

		glm::vec2 m_velocity;
		glm::vec2 m_acceleration;

		glm::vec2 m_force;
	};
}

#endif