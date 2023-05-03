#include "PhysicsBody.h"
PhysicsBody::PhysicsBody(glm::vec2* pos, float rot, glm::vec2* sca, float mass, float density, float restitution, bool isStatic)
	: position(pos), rotation(rot), scale(sca), velocity(glm::vec2(0.0f)), force(glm::vec2(0.0f)), gravity(glm::vec2(0.0f)), mass(mass), density(density), restitution(restitution), isStatic(isStatic), collider(new PolygonCollider(pos, &rotation, sca)){
	area = 0;
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
