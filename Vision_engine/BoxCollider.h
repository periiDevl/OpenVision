#ifndef BOX_COLLIDER_CLASS_H
#define BOX_COLLIDER_CLASS_H

#include "Collider.h"
#include <glad/glad.h>


class BoxCollider : public Collider
{
public:
	BoxCollider(vec3* pos, float* rot, vec3* sca);
	virtual vec2 GetSupportPoint(vec2 direction);
	bool CollidesWithBox(BoxCollider* other, glm::vec2& mtv);
	vector<vec2> GetTransformedVertices();
	vec2 Min, Max;


private:
	vector<vec2> vertices;
	
};

#endif