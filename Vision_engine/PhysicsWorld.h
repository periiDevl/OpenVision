#ifndef PHYSICS_WORLD_CLASS_H
#define PHYSICS_WORLD_CLASS_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <vector>

#include "PhysicsBody.h"

using namespace std;
using namespace glm;


class PhysicsWorld
{
public:
	PhysicsWorld(vec3 gravity, int iterations);
	void Step(float deltaTime);
	void AddBody(PhysicsBody* body);

private:
	int iterations;
	vec3 gravity;
	vector<PhysicsBody*> bodies;

};
#endif
