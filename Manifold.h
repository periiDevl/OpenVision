#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <glm/glm.hpp>

class PhysicsBody;

namespace physics2D
{
	struct Manifold
	{
		Manifold() {}

		glm::vec2 normal;
		double depth;

		glm::vec2 mtv;

		PhysicsBody* body1;
		PhysicsBody* body2;
	};
}

#endif