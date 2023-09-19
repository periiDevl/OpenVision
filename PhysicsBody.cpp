#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(glm::vec2* pos, float* rot, glm::vec2* sca, float mass, float density, float fric, float restitution, bool isTrigger, bool isStatic)
	: position(pos), rotation(rot), scale(sca), velocity(glm::vec2(0.0f)), force(glm::vec2(0.0f)), gravity(glm::vec2(0.0f)), mass(mass), density(density), restitution(restitution), friction(fric), isTrigger(isTrigger), isStatic(isStatic), collider(new PolygonCollider(pos, rot, sca)) {
	area = 0;
	layer = 0;

	angularInertia = (1.0f/12.0f) * mass * (sca->x * sca->x + sca->y * sca->y);
}
/*

	vec2* position;
	vec2* scale;
	float* rotation;

	float mass, area, density, friction, restitution;
	bool isTrigger, isStatic;
	int layer;

	float angularInertia, angularVelocity;

	vec2 getVelocity();
	vec2 force;

private:
	vec2 gravity;
	Collider* collider;
	vec2 velocity;
*/
void PhysicsBody::Values() {
	cout << "position:" << glm::to_string(*position) << endl;
	cout << "scale:" << glm::to_string(*scale) << endl;
	cout << "rotation:" << *rotation << endl;
	
	cout << "mass:" << (mass) << endl;
	cout << "area:" << (area) << endl;
	cout << "density:" << (density) << endl;
	cout << "friction:" << (friction) << endl;
	cout << "restitution:" << (restitution) << endl;

	cout << "trigger:" << (isTrigger) << endl;
	cout << "static:" << (isStatic) << endl;
	
	cout << "layer:" << layer << endl;

	cout << "velocity:" << glm::to_string(velocity) << endl;
	cout << "gravity:" << glm::to_string(gravity) << endl;
	cout << "force:" << glm::to_string(force) << endl;

	cout << "angular intertia:" << (angularInertia) << endl;
	cout << "angular velocity:" << (angularVelocity) << endl;
}

void PhysicsBody::Step(float deltaTime){

	*position += velocity * deltaTime;
	velocity += gravity * deltaTime;
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

void PhysicsBody::SetAngularVelocity(const float newAngularVelocity)
{
	if (isStatic)
		return;

	angularVelocity = newAngularVelocity;
}

vec2 PhysicsBody::getVelocity()
{
	return velocity;
}
