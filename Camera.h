#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "EventManager.h"
#include "Window.h"
#include "Shader.h"

class Camera3D
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::vec3 trackballPos;
	bool firstClick = true;
	glm::vec3 getDirection(glm::vec3 direction, bool vertical)
	{
		if (vertical)
		{
			direction = glm::vec3(direction.x, 0, direction.z);
		}
		if (glm::length(direction) > 0)
		{
			direction = glm::normalize(direction);
		}
		return direction;
	}

	int width = 1920;
	int height = 1080;

	float speed = 0.1f;
	float sensitivity = 100.0f;

	Camera3D(int width, int height, glm::vec3 position);
	void updateMatrix3D(float FOVdeg, float nearPlane, float frPlane);
	void updateMatrix2D(float scale, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);
	void Trackaballmovement(GLFWwindow* window, float normalSpeed, float highSpeed);
	void Inputs(GLFWwindow* window, float ctrlSpeed, float norSpeed);
	void Mouse(GLFWwindow* window);
	void TrackBallMouse(GLFWwindow* window);
	glm::vec3 screenToWorldRay(float mouseX, float mouseY) {
		// Convert mouse coordinates to normalized device coordinates (NDC)
		float x = (2.0f * mouseX) / width - 1.0f;
		float y = 1.0f - (2.0f * mouseY) / height; // Invert y-axis to match OpenGL's convention

		// Create the Ray in world space (start and direction)
		glm::vec4 ray_clip(x, y, -1.0f, 1.0f); // z = -1 (near plane), w = 1 (homogeneous coordinates)

		// Calculate the inverse of the projection * view matrix
		glm::mat4 inverse_proj_view = glm::inverse(cameraMatrix);

		// Transform the ray from clip space to world space
		glm::vec4 ray_eye = inverse_proj_view * ray_clip;
		ray_eye /= ray_eye.w; // Normalize by the w component (homogeneous division)

		// Calculate the direction of the ray (normalized)
		glm::vec3 ray_world = glm::normalize(glm::vec3(ray_eye));

		// Ray origin is the camera's position
		glm::vec3 ray_origin = Position;

		// Return the ray origin and direction as a pair (or modify as needed)
		return ray_origin + ray_world * 100.0f;  // Adjust 100.0f based on desired range
	}

private:
	glm::vec2 mouse;
};
#endif