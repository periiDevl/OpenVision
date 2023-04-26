#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(vec2 pos, float rot, vec2 sca, float mass, float area, float density, float restitution, float inertia, bool isStatic)
{
	// Set Initalized Values
	this->position = pos;
	this->rotation = rot;
	this->scale = sca;

	// Reset Values
	this->velocity = vec2(0.0f);
	this->force = vec2(0.0f);
	this->angularVelocity = 0;
	this->gravity = vec2(0.0f);

	// Set Body Propeties
	this->mass = mass;
	this->area = area;
	this->density = density;
	this->restitution = restitution;
	this->inertia = inertia;
	this->isStatic = isStatic;
	this->collider = new PolygonCollider(& position, & rotation, & scale);
}

void PhysicsBody::Step(float deltaTime)
{
	position += velocity   * deltaTime;
	force += gravity * mass;
	velocity += force / mass * deltaTime;
	force = vec2(0.0f);

}

vec2 PhysicsBody::GetPosition()
{
	return position;
}

float PhysicsBody::GetRotation()
{
	return rotation;
}

vec2 PhysicsBody::GetScale()
{
	return scale;
}

float PhysicsBody::GetMass()
{
	return mass;
}

float PhysicsBody::GetArea()
{
	return area;
}

float PhysicsBody::GetDensity()
{
	return density;
}

float PhysicsBody::GetRestituion()
{
	return restitution;
}

float PhysicsBody::GetInertia()
{
	return inertia;
}

bool PhysicsBody::GetIsStatic()
{
	return isStatic;
}

Collider* PhysicsBody::GetCollider()
{
	return collider;
}

vec2 PhysicsBody::GetVelocity()
{
	return velocity;
}

void PhysicsBody::ApplyForce(vec2 newForce)
{
	force += newForce;
}

void PhysicsBody::SetVelocity(vec2 newVelocity)
{
	if (!isStatic)
		velocity = newVelocity;
}

void PhysicsBody::SetPosition(vec2 newPosition)
{
	position = newPosition;
}

void PhysicsBody::SetGravity(vec2 newGravity)
{
	gravity = newGravity;
}
