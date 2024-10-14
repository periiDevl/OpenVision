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

	Simplex()
	{
		length = 0;
	}

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

	glm::vec2& operator[](int index)
	{
		return simplex[index];
	}

	const std::array<glm::vec2, SIMPLEX_SIZE>::const_iterator& begin() const
	{
		return simplex.begin();
	}

	const std::array<glm::vec2, SIMPLEX_SIZE>::const_iterator& end() const
	{
		return simplex.begin() + length;
	}

private:
	int length;
	std::array<glm::vec2, SIMPLEX_SIZE> simplex;
};

#endif