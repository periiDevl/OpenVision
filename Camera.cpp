#include "Camera.h"


Camera::Camera(glm::vec3 position, int width = 800, int height = 800) : position(position), width(width), height(height)
{
    updateMatrix(0.1, 1000);
}

void Camera::updateMatrix(float nearPlane, float farPlane)
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

void Camera::setAspectRatio(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    updateMatrix(0.1, 100); // Recalculate the camera matrix with the new aspect ratio
}

void Camera::Matrix(GLuint shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

