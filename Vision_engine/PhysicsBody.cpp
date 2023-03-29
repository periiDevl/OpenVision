#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(vec2 pos, float rot, vec2 sca, float mass, float area, float density, float restitution, float inertia)
{
	// Set Initalized Values
	this->position =  vec3(pos.x, pos.y, 0);
	this->rotation = rot;
	this->scale = vec3(sca.x, sca.y, 0);

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
	this->collider = new BoxCollider(&position, &rotation, &scale);
}

void PhysicsBody::Step(float deltaTime)
{
	position += vec3(velocity.x, velocity.y, 0) * deltaTime;
	force += gravity * mass;
	velocity += force / mass * deltaTime;
	force = vec2(0.0f);

}

vec3 PhysicsBody::GetPosition()
{
	return position;
}

float PhysicsBody::GetRotation()
{
	return rotation;
}

vec3 PhysicsBody::GetScale()
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
	velocity = newVelocity;
}

void PhysicsBody::SetPosition(vec2 newPosition)
{
	position = vec3(newPosition.x, newPosition.y, 0.0f);
}

void PhysicsBody::SetGravity(vec2 newGravity)
{
	gravity = newGravity;
}
