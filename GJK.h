#ifndef GJK_H
#define GJK_H

#include "BaseCollider.h"
#include "Simplex.h"
#include "PhysHelper.h"


class GJK
{

public:
	static bool isTouching(const BaseCollider& collA, const BaseCollider& collB)
	{
		// direction will be used for the entire of this function
		glm::vec2 direction = glm::normalize(collB.position - collA.position);

		Simplex simplex(support(collA, collB, direction));

		// same as vec2(0) - simplex.simplex[0]
		direction = -simplex.simplex[0];

		while (true)
		{
			glm::vec2 newPoint = support(collA, collB, direction);

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

	static bool originInSimplex(Simplex& simplex, glm::vec2& direction)
	{
		switch (simplex.getState())
		{
			case Simplex::SimplexState::Line:
			{
				glm::vec2 ab = simplex.simplex[0] - simplex.simplex[1];
				glm::vec2 ao = - simplex.simplex[1];

				// check if directions of ao and ab are the same, I.E origin is on the line
				float dotProduct = glm::dot(ao, ab);
				float aoLengthSquared = glm::dot(ao, ao);
				float abLengthSquared = glm::dot(ab, ab);

				if ((dotProduct * dotProduct) == (aoLengthSquared * abLengthSquared))
				{
					return true;
				}

				// set the next direction based on ab and ao
				direction = glm::normalize(tripleProduct(ab, ao, ab));

				return false;
			}
			case Simplex::SimplexState::Triangle:
			{
				glm::vec2& c = simplex.simplex[0];
				glm::vec2& b = simplex.simplex[1];
				glm::vec2& a = simplex.simplex[2];

				glm::vec2 ab = b - a;
				glm::vec2 ao =   - a;
				glm::vec2 ac = c - a;

				glm::vec2 abPerp = tripleProduct(ac, ab, ab);

				if (glm::dot(abPerp, ao) > 0)
				{
					// C is removed
					simplex.setLine(0, 1);

					direction = glm::normalize(abPerp);
					return false;
				}

				glm::vec2 acPerp = tripleProduct(ab, ac, ac);

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

};

#endif