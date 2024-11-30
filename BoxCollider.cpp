#include "BoxCollider.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace physics2D;

BoxCollider::BoxCollider(const glm::vec2 position, const float rotation, glm::vec2 scale)
	: BaseCollider(position, rotation), m_scale(scale )
{ }

glm::vec2 BoxCollider::findFurthestPoint(const glm::vec2 & direction) const
{
	glm::vec2 points[BOX_POINTS_SIZE];

	float sinRot = sin(m_rotation * DEG_TO_RED);
	float cosRot = cos(m_rotation * DEG_TO_RED);

	points[0] = { m_scale.x ,  m_scale.y };
	points[1] = { -m_scale.x ,  m_scale.y };
	points[2] = { -m_scale.x , -m_scale.y };
	points[3] = { m_scale.x , -m_scale.y };

	for (unsigned short i = 0; i < BOX_POINTS_SIZE; i++)
	{
		points[i] = { points[i].x * cosRot - points[i].y * sinRot,
					  points[i].x * sinRot + points[i].y * cosRot };
	}
	unsigned short maxIndex = 0;
	double maxDot = -DBL_MAX;
	for (unsigned short i = 0; i < BOX_POINTS_SIZE; i++)
	{
		double curDot = glm::dot(direction, points[i]);
		if (curDot > maxDot)
		{
			maxDot = curDot;
			maxIndex = i;
		}
	}

	std::cout << glm::to_string(points[maxIndex]) << ", " << glm::to_string(m_position) << std::endl;

	return points[maxIndex] + m_position;
}
