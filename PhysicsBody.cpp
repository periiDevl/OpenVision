#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(glm::vec2* pos, float* rot, glm::vec2* sca, float mass, float density, float fric, float restitution, bool isTrigger, bool isStatic)
	: position(pos), rotation(rot), scale(sca), linearVelocity(glm::vec2(0.0f)), force(glm::vec2(0.0f)), gravity(glm::vec2(0.0f)), mass(mass), density(density), restitution(restitution), friction(fric), isTrigger(isTrigger), isStatic(isStatic), collider(new PolygonCollider(pos, rot, sca)) {
	area = 0;
	layer = 0;

	inertia = (1.0f/12.0f) * mass * (sca->x * sca->x + sca->y * sca->y);

	if (!isStatic) {
		invMass = 1 / mass;
		invInertia = 1 / inertia;
	}
	else {
		invMass = 0;
		invInertia = 0;
	}
}
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

	cout << "velocity:" << glm::to_string(linearVelocity) << endl;
	cout << "gravity:" << glm::to_string(gravity) << endl;
	cout << "force:" << glm::to_string(force) << endl;

	cout << "angular intertia:" << (inertia) << endl;
	cout << "angular velocity:" << (angularVelocity) << endl;
}

void PhysicsBody::Step(float deltaTime){

	*position += linearVelocity * deltaTime;
	linearVelocity += gravity * deltaTime;
	linearVelocity += force / mass * deltaTime ;
	force = vec2(0.0f);

	*rotation += angularVelocity * deltaTime;// *0.0174533f;
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

vec2 PhysicsBody::LinearVelocity()
{
	return linearVelocity;
}
vec2 PhysicsBody::LinearVelocity(const vec2 value)
{
	if (isStatic)
		return vec2(0);

	if (linearVelocity == value)
		return linearVelocity;

	linearVelocity = value;
	return linearVelocity;
}
vec2 PhysicsBody::AddLinearVelocity(const vec2 addedValue)
{
	if (isStatic)
		return vec2(0);

	linearVelocity += addedValue;
	return linearVelocity;
}

float PhysicsBody::AngularVelocity()
{
	return angularVelocity;
}
float PhysicsBody::AngularVelocity(const float value)
{
	if (isStatic)
		return 0;

	if (angularVelocity == value)
		return angularVelocity;

	angularVelocity = value;
	return angularVelocity;
}
float PhysicsBody::AddAngularVelocity(const float addedValue)
{
	angularVelocity += addedValue;
	return angularVelocity;
}

float PhysicsBody::Mass()
{
	return mass;
}
float PhysicsBody::Mass(const float value)
{
	if (mass == value)
		return mass;
	mass = value;
	inertia = (1.0f / 12.0f) * mass * (scale->x * scale->x + scale->y * scale->y);

	if (!isStatic) {
		invMass = 1 / mass;
		invInertia = 1 / inertia;
	}
	else {
		invMass = 0;
		invInertia = 0;
	}

	return mass;
}

float PhysicsBody::Inertia()
{
	if (isStatic) {
		return 0;
	}
	return inertia;
}
float PhysicsBody::Inertia(const float value)
{
	if (inertia == value)
		return inertia;

	// Change mass based on that which will change the inertia as well
	// 1/12 * x * (width*width + height*height) = inertia
	// x = 12 * (i / (width * width + height * height)) (it took me 20 minutes to solve this equation)
	
	Mass(12 * (value / (scale->x * scale->x + scale->y * scale->y)));

	return inertia;
}

float PhysicsBody::InvInertia()
{
	if (isStatic) {
		return 0;
	}
	return inertia;
}

float PhysicsBody::InvMass()
{
	if (isStatic) {
		return 0;
	}
	return invMass;
}
float PhysicsBody::InvMass(const float value)
{
	if ((1.0f/invMass) == mass)
		return invMass;

	// If setting the inverse mass to a different value, change the mass and thus the inverse mass
	Mass((1.0f / invMass));
	
	return invMass;
}

bool PhysicsBody::IsTrigger()
{
	return isTrigger;
}
bool PhysicsBody::IsTrigger(bool value)
{
	if (isTrigger == value)
		return isStatic;

	isTrigger = value;
	return isTrigger;
}

bool PhysicsBody::IsStatic()
{
	return isStatic;
}
bool PhysicsBody::IsStatic(bool value)
{
	if (isStatic == value)
		return isStatic;

	isStatic = value;

	if (!isStatic) {
		invMass = 1 / mass;
		invInertia = 1 / inertia;
	}
	else {
		invMass = 0;
		invInertia = 0;
	}

	return isStatic;
}


