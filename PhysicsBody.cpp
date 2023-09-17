#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(glm::vec2* pos, float* rot, glm::vec2* sca, float mass, float density, float fric, float restitution, bool isTrigger, bool isStatic)
	: position(pos), rotation(rot), scale(sca), velocity(glm::vec2(0.0f)), force(glm::vec2(0.0f)), gravity(glm::vec2(0.0f)), mass(mass), density(density), restitution(restitution), friction(fric), isTrigger(isTrigger), isStatic(isStatic), collider(new PolygonCollider(pos, rot, sca)) {
	area = 0;
	layer = 0;

	angularInertia = (1.0f/12.0f) * mass * (sca->x * sca->x + sca->y * sca->y);
}

void PhysicsBody::Step(float deltaTime){
	*position += velocity * deltaTime;
	force += gravity * mass;
	velocity += force / mass * deltaTime ;
	force = vec2(0.0f);

	*rotation += angularVelocity * deltaTime;
}

Collider* PhysicsBody::GetCollider(){
	return collider;
}
void PhysicsBody::ApplyForce(const vec2 newForce){
	force += newForce;
}
void PhysicsBody::SetGravity(const vec2 newGravity){
	gravity = newGravity;
}

void PhysicsBody::SetCollider(Collider* newCollider)
{
	collider = newCollider;
}

void PhysicsBody::SetVelocity(const vec2 newVelocity)
{
	if (isStatic)
		return;
	
	velocity = newVelocity;
}

vec2 PhysicsBody::getVelocity()
{
	return velocity;
}
