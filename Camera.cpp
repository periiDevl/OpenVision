#include"Camera.h"



Camera3D::Camera3D(int width, int height, glm::vec3 position)
{
	Camera3D::width = width;
	Camera3D::height = height;
	Position = position;
}


void Camera3D::updateMatrix2D(float scale, float nearPlane, float farPlane) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	float aspectRatio = (float)width / (float)height;
	float left = -width / 2 * aspectRatio * scale;
	float right = width / 2 * aspectRatio * scale;
	float bottom = -height / 2 * scale;
	float top = height / 2 * scale;
	projection = glm::ortho(left, right, bottom, top, nearPlane, farPlane);

	cameraMatrix = projection * view;
}





void Camera3D::updateMatrix3D(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}
void Camera3D::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera3D::Mouse(GLFWwindow* window)
{

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

	
}


void Camera3D::TrackBallMouse(GLFWwindow* window)
{

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 centerPoint = glm::vec3(Position.x, Position.y, Position.z) + glm::vec3(Orientation.x * 100, Orientation.y * 100, Orientation.z * 100);

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		newOrientation = glm::rotate(newOrientation, glm::radians(-rotY), Up);

		if (abs(glm::angle(newOrientation, Up) - glm::radians(180.0f)) <= glm::radians(180.0f))
		{
			Orientation = newOrientation;
		}

		glm::vec3 newPos = glm::rotate(Position - centerPoint, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up))) + centerPoint;
		newPos = glm::rotate(newPos - centerPoint, glm::radians(-rotY), Up) + centerPoint;
		trackballPos = newPos;
		Position = newPos;





		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		firstClick = true;
	}

}


void Camera3D::Inputs(GLFWwindow* window, float normalSpeed, float highSpeed)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * getDirection(Orientation, false);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -getDirection(Orientation, false);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -getDirection(glm::cross(Orientation, Up), false);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * getDirection(glm::cross(Orientation, Up), false);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * getDirection(Up, false);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -getDirection(Up, false);
	}


	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = highSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = normalSpeed;
	}
	float rotationAmount = 0.7f;  // Adjust rotation sensitivity as needed
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(rotationAmount), Up);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(-rotationAmount), Up);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(rotationAmount), glm::normalize(glm::cross(Orientation, Up)));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(-rotationAmount), glm::normalize(glm::cross(Orientation, Up)));
	}
}
void Camera3D::Trackaballmovement(GLFWwindow* window, float normalSpeed, float highSpeed)
{

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * getDirection(Orientation, false);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -getDirection(Orientation, false);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -getDirection(glm::cross(Orientation, Up), false);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * getDirection(glm::cross(Orientation, Up), false);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * getDirection(Up, false);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -getDirection(Up, false);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = highSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = normalSpeed;
	}


}