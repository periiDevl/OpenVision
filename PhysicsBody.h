#ifndef PHYSICS_BODY_CLASS_H
#define PHYSICS_BODY_CLASS_H


#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <iostream>
#include <vector>
#include "Collider.h"
#include "PolygonCollider.h"
#include "CircleCollider.h"


using namespace glm;
using namespace std;

class PhysicsBody
{
public:
	PhysicsBody();
	PhysicsBody(glm::vec2* pos, float* rot, glm::vec2* sca, float mass, float density,float fric, float restitution,bool isTrigger, bool isStatic);

	
	void Step(float deltaTime);

	Collider* GetCollider();
	
	void ApplyForce(const vec2 newForce);

	void SetGravity(const vec2 newGravity);
	void SetCollider(Collider* newCollider);
	void SetVelocity(const vec2 newVelocity);

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

};

#endif
