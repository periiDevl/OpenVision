#ifndef CAMERA_CLASS2D_H
#define CAMERA_CLASS2D_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "Window.h"

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
	void setVieportSize(int w, int h) { v_width = w; v_height = h; }
	glm::vec2 mouseAsWorldPosition(glm::vec2 viewportSize);
	glm::vec2 worldToScreen(glm::vec2 worldPosition);
	glm::vec2 screenToWorld(glm::vec2 screenPosition);
private:
	int v_width;
	int v_height;
	

};
#endif