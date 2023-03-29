#include "BoxCollider.h"

BoxCollider::BoxCollider(vec3* pos, float* rot, vec3* sca)
{
	Position = pos;
	Rotation = rot;
	Scale = sca;
	
	vertices = vector<vec2>(4);
	vertices[0] = vec2(pos->x + sca->x/2, pos->y) ;
	vertices[1] = vec2(pos->x - sca->x/2, pos->y) ;
	vertices[2] = vec2(pos->x, pos->y + sca->y/2) ;
	vertices[3] = vec2(pos->x, pos->y - sca->y/2) ;
}


vec2 BoxCollider::GetSupportPoint(vec2 direction) {
	vector<vec2> transformedVertices = GetTransformedVertices();
	float maxDis = -FLT_MAX;
	vec2 finalVertex = vec3(0.0f);

	for (const auto& vertex : vertices) {

		float dis = dot(vertex + vec2(*Position), direction);
		cout << "Dis: " << dis << " Ver:" << to_string(vertex + vec2(*Position)) << " Dir:" << to_string(direction) << endl;

		if (dis > maxDis) {
			maxDis = dis;
			finalVertex = vertex;
		}
	}
	return finalVertex + vec2(*Position);
}

vector<vec2> BoxCollider::GetTransformedVertices()
{
	quat rotation(vec3(0.0f, 0.0f, *Rotation));
	mat4 transform = translate(mat4(1.0f), *Position) *
					 toMat4(rotation) *
					 scale(glm::mat4(1.0f), *Scale);
	
	vector<vec2> transformedVertices;
	
	for (const auto& vertex : vertices) {
		vec4 transformedVertex = transform * vec4(vec3(vertex, 0), 1.0f);
		transformedVertices.push_back(vec3(transformedVertex));
	}
	return transformedVertices;
}

/// <summary>
/// We create an AABB collision detection
/// </summary>
bool BoxCollider::CollidesWithBox(BoxCollider* other, glm::vec2& mtv) {

	Min.x = Position->x - Scale->x/2;
	Max.x = Position->x + Scale->x/2;
	Min.y = Position->y - Scale->y/2;
	Max.y = Position->y + Scale->y/2;
	

	other->Min.x = other->Position->x - other->Scale->x/2;
	other->Max.x = other->Position->x + other->Scale->x/2;
	other->Min.y = other->Position->y - other->Scale->y/2;
	other->Max.y = other->Position->y + other->Scale->y/2;

	bool collisionX = Min.x < other->Max.x && Max.x > other->Min.x;
	bool collisionY = Min.y < other->Max.y && Max.y > other->Min.y;

	if (collisionX && collisionY) {
		// Calculate the MTV in each axis
		if (Max.x > other->Max.x) {
			mtv.x = other->Max.x - Min.x;
		}
		else {
			mtv.x = other->Min.x - Max.x;
		}

		if (Max.y > other->Max.y) {
			mtv.y = other->Max.y - Min.y;
		}
		else {
			mtv.y = other->Min.y - Max.y;
		}

		// Find the axis with the smallest overlap and set the MTV in that axis
		if (std::abs(mtv.x) <= std::abs(mtv.y))
			mtv = vec2(mtv.x, 0.0f);
		else
			mtv = vec2(0.0f, mtv.y);

		return true;
	}

	return false;
}