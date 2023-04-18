#include "PolygonCollider.h"

PolygonCollider::PolygonCollider(vec2* pos, float* rot, vec2* sca, vector<vec2> vertices) {
	
	this->vertices = vertices;

	Position = pos;
	Rotation = rot;
	Scale = sca;

	bMin = vec2(-sca->x / 2, -sca->y / 2);
	bMax = vec2(sca->x / 2, sca->y / 2);
	bRadius = distance(vec2(0.0f), bMax);
}

PolygonCollider::PolygonCollider(vec2* pos, float* rot, vec2* sca)
{
	Position = pos;
	Rotation = rot;
	Scale = sca;

	vertices = vector<vec2>();
	vertices.push_back(vec2( sca->x / 2,  sca->y / 2));
	vertices.push_back(vec2(-sca->x / 2,  sca->y / 2));
	vertices.push_back(vec2(-sca->x / 2, -sca->y / 2));
	vertices.push_back(vec2( sca->x / 2, -sca->y / 2));

	bMin = vec2(-sca->x / 2, -sca->y / 2);
	bMax = vec2(sca->x / 2, sca->y / 2);
	bRadius = distance(vec2(0.0f), bMax);
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
	quat rotation(vec3(0.0f, 0.0f, radians(*Rotation)));
	mat4 transform =
		scale(glm::mat4(1.0f), vec3(1.0f, 1.0f, 1.0f)) *
		toMat4(rotation) *
		translate(mat4(1.0f), vec3(Position->x, Position->y, 0));

	vector<vec2> transformedVertices;

	for (const auto& vertex : vertices) {
		vec4 transformedVertex = transform * vec4(vec3(vertex, 0), 1.0f);
		transformedVertices.push_back(vec2(transformedVertex));
	}
	return transformedVertices;
}

void PolygonCollider::CalculateAABB()
{
	bMax = vec2(0.0f);
	bMin = vec2(0.0f);
	for (size_t i = 0; i < vertices.size(); i++)
	{
		bMax.x = std::max(bMax.x, (vertices)[i].x);
		bMax.y = std::max(bMax.y, (vertices)[i].y);

		bMin.x = std::min(bMin.x, (vertices)[i].x);
		bMin.y = std::min(bMin.y, (vertices)[i].y);
	}
}

void PolygonCollider::CalculateBoundRadius()
{
	bRadius = 0.0f;
	for (size_t i = 0; i < vertices.size(); i++) {
		bRadius = std::max(bRadius, distance(vec2(0.0f), (vertices)[i]));
	}
}
