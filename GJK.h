#ifndef GJK_H
#define GJK_H

#include "BaseCollider.h"
#include "Simplex.h"
#include "PhysHelper.h"
namespace physics2D
{

	class GJK
	{

	public:
		static bool isTouching(const BaseCollider& collA, const BaseCollider& collB);

		static bool isTouching(const BaseCollider& collA, const BaseCollider& collB, Simplex& simplex);

	private:
		static bool originInSimplex(Simplex& simplex, glm::vec2& direction);
	};
}

#endif