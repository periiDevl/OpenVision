#include "PolygonCollider.h"

PolygonCollider::PolygonCollider(vec2* pos, float* rot, vec2* sca, vector<vec2> vertices) {
	
	this->vertices = vertices;

	Position = pos;
	Rotation = rot;
	Scale = sca;

	type = ColliderType::Polygon;

	CalculateAABB();
	CalculateBoundRadius();
}

PolygonCollider::PolygonCollider(vec2* pos, float* rot, vec2* sca)
{
	Position = pos;
	Rotation = rot;
	Scale = sca;


	vertices = vector<vec2>();
	vertices.push_back(vec2(-0.5f,  0.5f));
	vertices.push_back(vec2(-0.5f, -0.5f));
	vertices.push_back(vec2( 0.5f, -0.5f));
	vertices.push_back(vec2( 0.5f,  0.5f));

	type = ColliderType::Box;

	CalculateAABB();
	CalculateBoundRadius();
}
vector<vec2> PolygonCollider::GetTransformedVertices()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(Position->x, -Position->y, 0.0f));
	model = glm::rotate(model, radians(*Rotation), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(*Scale, 1.0f));

	vector<vec2> transformedVertices;

	for (const auto& vertex : vertices) {
		vec4 transformedVertex = model * vec4(vec3(vertex, 0), 1.0f);
		transformedVertices.push_back(vec2(transformedVertex.x, -transformedVertex.y));
	}
	return transformedVertices;
}

void PolygonCollider::CalculateAABB()
{
	bMax = vec2(-std::numeric_limits<float>::max());
	bMin = vec2( std::numeric_limits<float>::max());
	vector<vec2> newVertices = GetTransformedVertices();
	for (size_t i = 0; i < newVertices.size(); i++)
	{
		bMax.x = std::max(bMax.x, (newVertices)[i].x);
		bMax.y = std::max(bMax.y, (newVertices)[i].y);

		bMin.x = std::min(bMin.x, (newVertices)[i].x);
		bMin.y = std::min(bMin.y, (newVertices)[i].y);
	}
}

void PolygonCollider::CalculateBoundRadius()
{
	bRadius = 0.0f;
	vector<vec2> newVertices = GetTransformedVertices();

	for (size_t i = 0; i < newVertices.size(); i++) {
		bRadius = std::max(bRadius, distance(vec2(0.0f), (newVertices)[i]));
	}
}
