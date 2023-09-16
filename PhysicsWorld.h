#ifndef PHYSICS_WORLD_CLASS_H
#define PHYSICS_WORLD_CLASS_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <vector>
#include "Collider.h"
#include "PhysicsBody.h"
#include "CollisionManager.h"

using namespace std;
using namespace glm;


class PhysicsWorld
{
public:
	PhysicsWorld(vec3 gravity, int iterations);
	void Step(float deltaTime);
	void SetGravity(vec3 gravity);
	vec3 GetGravity();
	void AddBody(PhysicsBody* body);
	void RemoveBody(PhysicsBody* body);
	vector<PhysicsBody*> GetBodies();
	void SeperateBodies(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold);
	void SimpleResolution(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold);
	void RotationResolution(PhysicsBody* bodyA, PhysicsBody* bodyB, Manifold manifold);
	bool TouchingLayer(PhysicsBody* body, int layer);
	void UpdateLayerBodies();
private:
	int iterations;
	vec3 gravity;
	vector<PhysicsBody*> bodies;
	map<int, vector<PhysicsBody*>> layeredBodies;
	vector<int> layers;

};
#endif
