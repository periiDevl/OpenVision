#ifndef BASE_COLLIDER_H_CLASS
#define BASE_COLLIDER_H_CLASS

#include <glm/glm.hpp>

class BaseCollider
{
public:
	glm::vec2 position;

	virtual glm::vec2 findFurthestPoint(const glm::vec2& direction) const = 0;
};

#endif
