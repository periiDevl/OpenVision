#include "PhysicsBody.h"

using namespace physics2D;

void PhysicsBody::fixedUpdate(float deltaTime)
{
	// apply acceleration, velocity and change to position
	m_acceleration = m_force * m_invMass; // a = F / m
	m_velocity += m_acceleration * deltaTime; // v = v + a * dt
	m_collider.m_position += m_velocity * deltaTime; // x = x + v * dt

	// reset force
	m_force = glm::vec2(0.0f);
}

void PhysicsBody::mass(float val)
{
	m_mass = (val <= 0.0f) ? 0.0f : val;
	m_invMass = (val <= 0.0f) ? 0.0f : 1.0f / m_mass;
}

float PhysicsBody::mass() const
{
	return m_mass;
}

float PhysicsBody::invMass() const
{
	return m_invMass;
}

void PhysicsBody::applyImpulse(const glm::vec2 impulse)
{
	m_velocity += impulse;
}

glm::vec2 PhysicsBody::velocity()
{
	if (isStatic())
		return glm::vec2(0);

	return m_velocity;
}
void PhysicsBody::velocity(const glm::vec2 val)
{
	m_velocity = val;
}

void PhysicsBody::applyForce(const glm::vec2 force)
{
	m_force += force;
}

bool PhysicsBody::isStatic() const
{
	return m_isStatic;
}

void PhysicsBody::isStatic(bool val)
{
	m_isStatic = val;
}

BaseCollider& PhysicsBody::getCollider() const
{
	return m_collider;
}

