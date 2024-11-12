#include "Camera2D.h"


Camera2D::Camera2D(glm::vec3 position, int width = 800, int height = 800) : position(position), width(width), height(height)
{
    EventManager::addCallback<EventWindowResize>(
        [&](const EventWindowResize* eventResize)
        {
            setAspectRatio(eventResize->width, eventResize->height);
        }
    );

    updateMatrix(0.1, 1000);
}

void Camera2D::updateMatrix(float nearPlane, float farPlane)
{
    glm::mat4 view = glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Create an orthographic projection matrix
    float aspectRatio = (float)width / (float)height;
    float orthoLeft = -aspectRatio;
    float orthoRight = aspectRatio;
    float orthoBottom = -1.0f;
    float orthoTop = 1.0f;

    glm::mat4 projection = glm::ortho(orthoLeft / zoom, orthoRight / zoom, orthoBottom / zoom, orthoTop / zoom, nearPlane, farPlane);

    cameraMatrix = projection * view;
}

void Camera2D::setAspectRatio(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    updateMatrix(0.1, 100); // Recalculate the camera matrix with the new aspect ratio
}

void Camera2D::Matrix(GLuint shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

glm::vec2 Camera2D::mouseAsWorldPosition()
{
	glm::vec2 mousePos = InputSystem::getMousePosition();

	updateMatrix(0.1, 100);

	float ndcX = (2.0f * mousePos.x) / width - 1.0f;
	float ndcY = 1.0f - (2.0f * mousePos.y) / height; // Y is inverted in GLFW

	// Step 3: Transform NDC to world coordinates using the inverse projection matrix
	// Set up the orthographic projection matrix, as before
	float orthoSize = 1.0f; // Example orthographic size
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	// Invert the orthographic projection matrix to convert NDC to world coordinates
	glm::mat4 inverseProjection = glm::inverse(cameraMatrix);
	glm::vec4 ndcPosition = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
	glm::vec4 worldPosition = inverseProjection * ndcPosition;

	// Extract the X and Y world coordinates from the result
	glm::vec2 worldCoords = glm::vec2(worldPosition.x, worldPosition.y);

	return worldCoords;
}

glm::mat4 Camera2D::getViewMatrix()
{
    return glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
