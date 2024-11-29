#include "EPA.h"

using namespace physics2D;

Manifold EPA::getResolution(const BaseCollider& collA, const BaseCollider& collB, const Simplex& simplex)
{
	Manifold result;

	Polytope polytope(simplex);

	int iterations = 0;


	if (polytope.getLength() == 2)
	{
		const glm::vec2 a = polytope[0];
		const glm::vec2 b = polytope[1];

		glm::vec2 ab = b - a;
		glm::vec2 perpendicularDir = glm::normalize(glm::vec2(-ab.y, ab.x));

		glm::vec2 thirdPoint = PhysHelper::support(collA, collB, perpendicularDir);

		polytope.insertAt(thirdPoint, 2);
	}

	while (iterations < EPA_MAX_ITERATIONS)
	{
		iterations++;

		Edge edge = closestEdge(polytope);

		glm::vec2 point = PhysHelper::support(collA, collB, edge.normal);

		double dis = glm::dot(point, edge.normal);

		if (dis - edge.distance < EPA_TOLERANCE)
		{
			result.normal = -edge.normal;
			result.depth = dis;
			result.mtv = (float)result.depth * result.normal;

			return result;
		}
		else
		{
			polytope.insertAt(point, edge.index);
		}
	}

	result.normal = glm::vec2(0.0f, 0.0f);
	result.depth = 0.0;
	return result;
}

Edge EPA::closestEdge(const Polytope& polytope)

{
	Edge closest;
	closest.distance = DBL_MAX;

	for (int i = 0; i < polytope.getLength(); i++)
	{
		int j = i + 1 == polytope.getLength() ? 0 : i + 1;

		const glm::vec2& a = polytope[i];
		const glm::vec2& b = polytope[j];

		glm::vec2 ba = a - b;

		glm::vec2 normal = glm::normalize(PhysHelper::tripleProduct(ba, b, ba));

		double dis = glm::dot(normal, b);

		if (dis < closest.distance)
		{
			closest.distance = dis;
			closest.normal = normal;
			closest.index = j;
		}
	}

	return closest;
}