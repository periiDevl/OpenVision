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
	
	void ApplyForce(vec2 newForce);

	void SetGravity(vec2 newGravity);
	void SetCollider(Collider* newCollider);

	vec2* position;
	vec2* scale;
	float* rotation;

	float mass, area, density, friction, restitution;
	bool isTrigger, isStatic;
	int layer;

	vec2 force, velocity;

private:
	vec2 gravity;
	Collider* collider;

};

#endif
