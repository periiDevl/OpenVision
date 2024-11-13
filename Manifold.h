#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <glm/glm.hpp>
namespace physics2D
{
	struct Manifold
	{
		Manifold() {}

		glm::vec2 normal;
		double depth;

		glm::vec2 mtv;
	};
}

#endif