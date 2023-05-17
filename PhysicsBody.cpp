#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(glm::vec2* pos, float* rot, glm::vec2* sca, float mass, float density, float fric, float restitution, bool isTrigger, bool isStatic)
	: position(pos), rotation(rot), scale(sca), velocity(glm::vec2(0.0f)), force(glm::vec2(0.0f)), gravity(glm::vec2(0.0f)), mass(mass), density(density), restitution(restitution), friction(fric), isTrigger(isTrigger), isStatic(isStatic), collider(new PolygonCollider(pos, rot, sca)) {
	area = 0;
	layer = 0;
}

void PhysicsBody::Step(float deltaTime){
	*position += velocity * deltaTime;
	force += gravity * mass;
	velocity += force / mass * deltaTime;
	force = vec2(0.0f);

}

Collider* PhysicsBody::GetCollider(){
	return collider;
}
void PhysicsBody::ApplyForce(vec2 newForce){
	force += newForce;
}
void PhysicsBody::SetGravity(vec2 newGravity){
	gravity = newGravity;
}

void PhysicsBody::SetCollider(Collider* newCollider)
{
	collider = newCollider;
}
