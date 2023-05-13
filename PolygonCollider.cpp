#include "PolygonCollider.h"

PolygonCollider::PolygonCollider(vec2* pos, float* rot, vec2* sca, vector<vec2> vertices) {
	
	this->vertices = vertices;

	Position = pos;
	Rotation = rot;
	Scale = sca;

	bMin = vec2(-1 / 2, -1 / 2);
	bMax = vec2( 1 / 2,  1 / 2);
	bRadius = distance(vec2(0.0f), bMax);
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
										 
	CalculateAABB();
	CalculateBoundRadius();
}

vec2 PolygonCollider::GetSupportPoint(vec2 direction)
{
	vector<vec2> transformedVertices = GetTransformedVertices();
	float maxDis = -FLT_MAX;
	vec2 finalVertex = vec2(0.0f);


	for (const auto& vertex : transformedVertices) {

		float dis = dot(vertex, direction);

		if (dis > maxDis) {
			maxDis = dis;
			finalVertex = vertex;
		}
	}
	return finalVertex;
}

vector<vec2> PolygonCollider::GetTransformedVertices() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(Position->x, -Position->y, 0.0f));
	model = glm::rotate(model, radians(*Rotation), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(*Scale, 1.0f));

	vector<vec2> transformedVertices;

	for (const auto& vertex : vertices) {
		vec4 transformedVertex = model * vec4(vec3(vertex, 0), 1.0f);
		transformedVertices.push_back(vec2(transformedVertex));
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
	for (size_t i = 0; i < vertices.size(); i++) {
		bRadius = std::max(bRadius, distance(vec2(0.0f), (vertices)[i]));
	}
}
