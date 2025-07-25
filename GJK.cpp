#include "GJK.h"

using namespace physics2D;

bool GJK::isTouching(const BaseCollider& collA, const BaseCollider& collB)
{
	Simplex simplex;

	return isTouching(collA, collB, simplex);
}

bool GJK::isTouching(const BaseCollider& collA, const BaseCollider& collB, Simplex& simplex)
{
	// direction will be used for the entire of this function
	glm::vec2 direction = glm::normalize(collB.m_position - collA.m_position);

	simplex.add(PhysHelper::support(collA, collB, direction));

	// same as vec2(0) - simplex[0]
	direction = glm::normalize(-simplex[0]);

	while (true)
	{
		glm::vec2 newPoint = PhysHelper::support(collA, collB, direction);

		// new point is in the opposite of the origin
		if (glm::dot(newPoint, direction) < 0)
		{
			return false;
		}

		simplex.add(newPoint);

		if (originInSimplex(simplex, direction))
		{
			return true;
		}
	}
}

bool GJK::originInSimplex(Simplex& simplex, glm::vec2& direction)
{
	switch (simplex.getState())
	{
	case Simplex::SimplexState::Line:
	{
		glm::vec2 ab = simplex[0] - simplex[1];
		glm::vec2 ao = -simplex[1];

		// set the next direction based on ab and ao
		direction = glm::normalize(PhysHelper::getPerpendicular(ab, ao));

		return false;
	}
	case Simplex::SimplexState::Triangle:
	{
		glm::vec2& c = simplex[0];
		glm::vec2& b = simplex[1];
		glm::vec2& a = simplex[2];

		glm::vec2 ab = b - a;
		glm::vec2 ao = -a;
		glm::vec2 ac = c - a;

		glm::vec2 abPerp = PhysHelper::tripleProduct(ac, ab, ab);

		if (glm::dot(abPerp, ao) > 0)
		{
			// C is removed
			simplex.setLine(0, 1);

			direction = glm::normalize(abPerp);
			return false;
		}

		glm::vec2 acPerp = PhysHelper::tripleProduct(ab, ac, ac);

		if (glm::dot(acPerp, ao) > 0)
		{
			// B is removed
			simplex.setLine(0, 2);

			direction = glm::normalize(acPerp);
			return false;
		}


		return true;

	}
	default:
		printf("It was not a line or a triangle!\n");
	}

	return false;
}
