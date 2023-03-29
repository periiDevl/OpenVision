#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

using namespace glm;

class Camera
{
public:
	vec3 Position;
	mat4 cameraMatrix = mat4(1.0f);


	int width;
	int height;


	Camera(int width, int height, vec3 position);

	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(GLuint shader, const char* uniform);
	vec2 PixelToWorldPos(vec2 pixelPosition, GLFWwindow* window);
};
#endif