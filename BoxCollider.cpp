#include "BoxCollider.h"

using namespace physics2D;

BoxCollider::BoxCollider(const glm::vec2 position, const float rotation, glm::vec2 scale)
	: BaseCollider(position, rotation), m_scale(scale * 2.0f)
{ }

glm::vec2 BoxCollider::findFurthestPoint(const glm::vec2 & direction) const
{
	glm::vec2 points[BOX_POINTS_SIZE];

	float sinRot = sin(m_rotation * DEG_TO_RED);
	float cosRot = cos(m_rotation * DEG_TO_RED);

	points[0] = { m_scale.x * 0.5f,  m_scale.y * 0.5f };
	points[1] = { -m_scale.x * 0.5f,  m_scale.y * 0.5f };
	points[2] = { -m_scale.x * 0.5f, -m_scale.y * 0.5f };
	points[3] = { m_scale.x * 0.5f, -m_scale.y * 0.5f };

	for (unsigned short i = 0; i < BOX_POINTS_SIZE; i++)
	{
		points[i] = { points[i].x * cosRot - points[i].y * sinRot + m_position.x,
					  points[i].x * sinRot + points[i].y * cosRot + m_position.y };
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

	return points[maxIndex] + m_position;
}
