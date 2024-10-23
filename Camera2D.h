#ifndef CAMERA_CLASS2D_H
#define CAMERA_CLASS2D_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Camera2D
{
public:
	glm::vec3 position;
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	float zoom = 1;
	int width;
	int height;


	Camera2D(glm::vec3 position, int width, int height);
	glm::mat4 getViewMatrix();
	void setAspectRatio(int newWidth, int newHeight);
	glm::mat4 getProjectionMatrix(float FOVdeg, float nearPlane, float farPlane);
	void updateMatrix(float nearPlane, float farPlane);
	void Matrix(GLuint shader, const char* uniform);
};
#endif