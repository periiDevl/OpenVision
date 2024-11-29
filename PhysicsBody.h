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
		
		void fixedUpdate(float deltaTime);

		void mass(float val);
		float mass() const;
		float invMass() const;

		void applyImpulse(const glm::vec2 impulse);
		glm::vec2 velocity();
		void velocity(const glm::vec2 val);

		void applyForce(const glm::vec2 force);

		bool isStatic() const;
		void isStatic(bool val);

		BaseCollider& getCollider() const;

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