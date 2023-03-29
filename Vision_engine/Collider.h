#ifndef COLLIDER_CLASS_H
#define COLLIDER_CLASS_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>
#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Tex.h"

using namespace glm;
using namespace std;


class Collider
{
public:
	virtual vec2 GetSupportPoint(vec2 direction) = 0;

	vec3* Position;
	float* Rotation;
	vec3* Scale;
};

#endif
