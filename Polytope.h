#ifndef POLYTOPE_H
#define POLYTOPE_H

#include <vector>
#include <glm/glm.hpp>
#include "Simplex.h"

namespace physics2D
{

	struct Edge
	{
		Edge() : distance(0), index(-1), normal{0, 0} {}

		// distance from origin
		double distance;

		// the normal to the out of the shape
		glm::vec2 normal;

		// index of the second point on the polytope
		int index;
	};

	class Polytope
	{
	public:

		Polytope(const Simplex& simplex) : points(simplex.begin(), simplex.end())
		{ }

		void insertAt(const glm::vec2& newPoint, const int& index)
		{
			points.emplace(points.begin() + index, newPoint);
		}

		std::size_t getLength() const
		{
			return points.size();
		}

		const glm::vec2& operator[](int index) const
		{
			return points[index];
		}

	private:
		std::vector<glm::vec2> points;
	};

}

#endif