#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <glm/glm.hpp>
#include <array>

const int SIMPLEX_SIZE = 3;

struct Simplex
{
public:
	enum class SimplexState
	{
		Dot = 1,
		Line = 2,
		Triangle = 3,
	};

	Simplex(const glm::vec2& point) : length(1)
	{
		simplex[0] = point;
	}

	void add(const glm::vec2& point)
	{
		if (length != SIMPLEX_SIZE)
		{
			simplex[length++] = point;
		}
	}

	void removeAt(int index)
	{
		for (int i = index; i < length - 1; i++)
		{
			simplex[i] = simplex[i + 1];
		}
	
		length--;

		// there will be empty data at the last point
	}

	void setLine(int indexA, int indexB)
	{
		simplex[0] = simplex[indexA];
		simplex[1] = simplex[indexB];
		length = 2;
	}

	SimplexState getState() const 
	{
		return (SimplexState)length;
	}

	int length;
	std::array<glm::vec2, SIMPLEX_SIZE> simplex;
};

#endif