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
#include "BoxCollider.h"

using namespace std;
using namespace glm;

class PhysicsBody
{
public:
	PhysicsBody(vec2 pos, float rot, vec2 sca, float mass, float area, float density, float restitution, float inertia);

	void Step(float deltaTime);

	vec3 GetPosition();
	float GetRotation();
	vec3 GetScale();
	float GetMass();
	float GetArea();
	float GetDensity();
	float GetRestituion();
	float GetInertia();

	Collider* GetCollider();
	
	vec2 GetVelocity();

	void ApplyForce(vec2 newForce);
	void SetVelocity(vec2 newVelocity);
	void SetPosition(vec2 newPosition);

	void SetGravity(vec2 newGravity);
private:
	vec3 position, scale;
	float rotation;


	float mass, area, density, restitution, inertia;

	vec2 force, velocity;
	float angularVelocity;
	
	
	vec2 gravity;
	Collider* collider;

};

#endif
