#ifndef EPA_CLASS_H
#define EPA_CLASS_H

#include <limits.h>
#include "Simplex.h"
#include "PhysHelper.h"
#include "Manifold.h"
#include "Polytope.h"
#include "BaseCollider.h"

#define EPA_TOLERANCE 0.00001
#define EPA_MAX_ITERATIONS 20

namespace physics2D
{

	class EPA
	{
	public:
		static Manifold getResolution(const BaseCollider& collA, const BaseCollider& collB, const Simplex& simplex);

		static Edge closestEdge(const Polytope& polytope);
	};
}
#endif