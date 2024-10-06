#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(glm::vec2* pos, float* rot, glm::vec2* sca, float mass, float density, float sFric, float dFric, float restitution, bool isTrigger, bool isStatic) 
	: position(pos), rotation(rot), scale(sca), m_linearVelocity(glm::vec2(0.0f)), m_force(glm::vec2(0.0f)), m_gravity(glm::vec2(0.0f)), m_mass(mass), density(density), restitution(restitution), staticFric(sFric), dynamicFric(dFric), m_isTrigger(isTrigger), m_isStatic(isStatic), m_collider(new PolygonCollider(pos, rot, sca))
{
	area = 0;
	layer = 0;

	m_inertia = (1.0f/12.0f) * mass * (sca->x * sca->x + sca->y * sca->y);

	if (!isStatic) {
		m_invMass = 1 / mass;
		m_invInertia = 1 / m_inertia;
	}
	else {
		m_invMass = 0;
		m_invInertia = 0;
	}
}
void PhysicsBody::Values() {
	cout << "position:" << glm::to_string(*position) << endl;
	cout << "scale:" << glm::to_string(*scale) << endl;
	cout << "rotation:" << *rotation << endl;
	
	cout << "mass:" << (m_mass) << endl;
	cout << "area:" << (area) << endl;
	cout << "density:" << (density) << endl;
	cout << "static friction:" << (staticFric) << endl;
	cout << "dynamic friction:" << (dynamicFric) << endl;
	cout << "restitution:" << (restitution) << endl;

	cout << "trigger:" << (m_isTrigger) << endl;
	cout << "static:" << (m_isStatic) << endl;
	
	cout << "layer:" << layer << endl;

	cout << "velocity:" << glm::to_string(m_linearVelocity) << endl;
	cout << "gravity:" << glm::to_string(m_gravity) << endl;
	cout << "force:" << glm::to_string(m_force) << endl;

	cout << "angular intertia:" << (m_inertia) << endl;
	cout << "angular velocity:" << (m_angularVelocity) << endl;
}

void PhysicsBody::step(float deltaTime){

	*position += m_linearVelocity * deltaTime;

	m_linearVelocity += m_gravity * deltaTime;
	
	m_linearVelocity += m_force / m_mass * deltaTime ;
	
	m_force = vec2(0.0f);

	*rotation += m_angularVelocity * 57.295779513082320876798154814105 * deltaTime;// *0.0174533f;
}

Collider& PhysicsBody::getCollider(){
	return m_collider;
}
void PhysicsBody::applyForce(const vec2 newForce){
	m_force += newForce;
}
void PhysicsBody::setGravity(const vec2 newGravity){
	m_gravity = newGravity;
}

void PhysicsBody::setCollider(Collider* newCollider)
{
	m_collider = newCollider;
}

vec2 PhysicsBody::LinearVelocity()
{
	return m_linearVelocity;
}
vec2 PhysicsBody::LinearVelocity(const vec2 value)
{
	if (m_isStatic)
		return vec2(0);

	if (m_linearVelocity == value)
		return m_linearVelocity;

	m_linearVelocity = value;
	return m_linearVelocity;
}
vec2 PhysicsBody::AddLinearVelocity(const vec2 addedValue)
{
	if (m_isStatic)
		return vec2(0);

	m_linearVelocity += addedValue;
	return m_linearVelocity;
}

float PhysicsBody::AngularVelocity()
{
	return m_angularVelocity;
}
float PhysicsBody::AngularVelocity(const float value)
{
	if (m_isStatic)
		return 0;

	if (m_angularVelocity == value)
		return m_angularVelocity;

	m_angularVelocity = value;
	return m_angularVelocity;
}
float PhysicsBody::AddAngularVelocity(const float addedValue)
{
	m_angularVelocity += addedValue;
	return m_angularVelocity;
}

float PhysicsBody::mass()
{
	if (m_isStatic)
		return 0;

	return m_mass;
}
float PhysicsBody::mass(const float value)
{
	if (m_mass == value)
		return m_mass;

	m_mass = value;
	m_inertia = (1.0f / 12.0f) * m_mass * (scale->x * scale->x + scale->y * scale->y);

	if (!m_isStatic) {
		m_invMass = 1 / m_mass;
		m_invInertia = 1 / m_inertia;
	}
	else {
		m_invMass = 0;
		m_invInertia = 0;
	}

	return m_mass;
}

float PhysicsBody::inertia()
{
	if (m_isStatic) {
		return 0;
	}
	return m_inertia;

}
float PhysicsBody::inertia(const float value)
{
	if (m_inertia == value)
		return m_inertia;

	// Change mass based on that which will change the inertia as well
	// 1/12 * x * (width*width + height*height) = inertia
	// x = 12 * (i / (width * width + height * height)) (it took me 20 minutes to solve this equation)
	
	mass(12 * (value / (scale->x * scale->x + scale->y * scale->y)));

	return m_inertia;
}

float PhysicsBody::invInertia()
{
	if (m_isStatic) {
		return 0;
	}
	return 1.0 / m_inertia;
}

float PhysicsBody::invMass()
{
	if (m_isStatic) {
		return 0;
	}
	return m_invMass;
}
float PhysicsBody::invMass(const float value)
{
	if ((1.0f/m_invMass) == m_mass)
		return m_invMass;

	// If setting the inverse mass to a different value, change the mass and thus the inverse mass
	mass((1.0f / m_invMass));
	
	return m_invMass;
}

bool PhysicsBody::isTrigger()
{
	return m_isTrigger;
}
bool PhysicsBody::isTrigger(bool value)
{
	if (m_isTrigger == value)
		return m_isStatic;

	m_isTrigger = value;
	return m_isTrigger;
}

bool PhysicsBody::isStatic()
{
	return m_isStatic;
}
bool PhysicsBody::isStatic(bool value)
{
	if (m_isStatic == value)
		return m_isStatic;

	m_isStatic = value;

	if (!m_isStatic) {
		m_invMass = 1.0 / m_mass;
		m_invInertia = 1.0 / m_inertia;
	}
	else {
		m_invMass = 0;
		m_invInertia = 0;
	}

	return m_isStatic;
}


