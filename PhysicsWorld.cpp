#include "PhysicsWorld.h"

void physics2D::PhysicsWorld::addBody(const PhysicsBody& body)
{
	bodies.push_back(body);
}