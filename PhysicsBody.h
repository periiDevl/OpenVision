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
	PhysicsBody(glm::vec2* pos, float* rot, glm::vec2* sca, float mass, float density, float sFric, float dFric, float restitution, bool isTrigger, bool isStatic);

	void Values();

	
	void Step(float deltaTime);

	Collider* GetCollider();
	
	void ApplyForce(const vec2 newForce);

	void SetGravity(const vec2 newGravity);
	void SetCollider(Collider* newCollider);


	/// <summary>
	/// A function for getting the current linear velocity (what did you expect?)
	/// </summary>
	/// <returns>a vec2 representing the linear velocity</returns>
	vec2 LinearVelocity();
	/// <summary>
	/// A function for setting the linear velocity  
	/// </summary>
	/// <returns>a vec2 representing the new linear velocity</returns>
	vec2 LinearVelocity(const vec2 value);
	/// <summary>
	/// A function for adding to the current linear velocity
	/// </summary>
	/// <returns>a vec2 representing the new linear velocity</returns>
	vec2 AddLinearVelocity(const vec2 addedValue);
	
	/// <summary>
	/// A function for getting the current angular velocity (what did you expect?)
	/// </summary>
	/// <returns>a float representing the angular velocity</returns>
	float AngularVelocity();
	/// <summary>
	/// A function for getting the current angular velocity
	/// </summary>
	/// <returns>a float representing the angular velocity</returns>
	float AngularVelocity(const float value);
	/// <summary>
	/// A function for getting the current angular velocity
	/// </summary>
	/// <returns>a float representing the angular velocity</returns>
	float AddAngularVelocity(const float addedValue);


	vec2* position;
	vec2* scale;
	float* rotation;

	float Mass();
	float Mass(const float value);

	float InvMass();
	float InvMass(const float value);


	float area, density, staticFric, dynamicFric, restitution;


	/// <summary>
	/// A function for getting the is-trigger property (what did you expect?)
	/// </summary>
	/// <returns>a bool representing the is-trigger property</returns>
	bool IsTrigger();
	/// <summary>
	/// A function for setting the is-trigger property
	/// </summary>
	/// <returns>a bool representing the is-trigger property</returns>
	bool IsTrigger(bool value);


	/// <summary>
	/// A function for getting the is-static property (what did you expect?)
	/// </summary>
	/// <returns>a bool representing the is-static property</returns>
	bool IsStatic();

	/// <summary>
	/// A function for setting the is-static property
	/// </summary>
	/// <returns>a bool representing the is-static property</returns>
	bool IsStatic(bool value);
	
	int layer;

	/// <summary>
	/// A function for getting the inertia (what did you expect?)
	/// </summary>
	/// <returns>a float representing the inertia</returns>
	float Inertia();

	/// <summary>
	/// A function for setting the inertia 
	/// </summary>
	/// <returns>a float representing the new inertia</returns>
	float Inertia(const float value);

	/// <summary>
	/// A function for getting 1/inertia (what did you expect?)
	/// </summary>
	/// <returns>a float representing the inertia</returns>
	float InvInertia();

	// Im not making another function for setting the mass using the inverse inertia, forget it

private:

	vec2 gravity;
	Collider* collider;
	vec2 force;
	vec2 linearVelocity;

	float angularVelocity;
	float invMass;
	float mass;
	float invInertia;
	float inertia;

	bool isTrigger, isStatic;
};

#endif
