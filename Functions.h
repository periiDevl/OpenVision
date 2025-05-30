#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <cstdlib>
#include <ctime>
#include <random>
class Functions {
public:
	bool ClickOnRGBID(GLFWwindow* window,int button,glm::vec3 rgb, glm::vec3 targetID) {
		//GLFW_MOUSE_BUTTON_LEFT
		if (rgb == targetID && glfwGetMouseButton(window, button) == GLFW_PRESS) {
			return true;
		}
		return false;
	}
	glm::quat Euler_to_quat(double roll, double pitch, double yaw)
	{
		double cr = cos(glm::radians(roll) * 0.5);
		double sr = sin(glm::radians(roll) * 0.5);
		double cp = cos(glm::radians(pitch) * 0.5);
		double sp = sin(glm::radians(pitch) * 0.5);
		double cy = cos(glm::radians(yaw) * 0.5);
		double sy = sin(glm::radians(yaw) * 0.5);

		glm::quat q;
		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;

		return q;
	}

	glm::vec3 Quat_to_euler(const glm::quat& q)
	{
		double roll = glm::degrees(atan2(2 * (q.w * q.x + q.y * q.z), 1 - 2 * (q.x * q.x + q.y * q.y)));
		double pitch = glm::degrees(asin(2 * (q.w * q.y - q.z * q.x)));
		double yaw = glm::degrees(atan2(2 * (q.w * q.z + q.x * q.y), 1 - 2 * (q.y * q.y + q.z * q.z)));

		return glm::vec3(roll, pitch, yaw);
	}



	glm::vec3 Direction_to_forward(glm::vec3 vector3, double yaw, double pitch)
	{
		vector3.x = cos(yaw) * cos(pitch);
		vector3.y = sin(yaw) * cos(pitch);
		vector3.z = sin(pitch);

		return vector3;
	}


	glm::quat QuatLookAt(
		glm::vec3 const& from,
		glm::vec3 const& to,
		glm::vec3 const& up
	)
	{
		glm::vec3  direction = to - from;
		float directionLength = glm::length(direction);

		if (!(directionLength > 0.0001))
			return glm::quat(1, 0, 0, 0);

		direction /= directionLength;

		if (glm::abs(glm::dot(direction, up)) > .9999f) {
			return glm::quatLookAt(direction, glm::vec3(0));
		}
		else {
			return glm::quatLookAt(direction, up);
		}
	}

	float Lerp(float from, float to, float time) {
		return from + time * (to - from);
	}

	glm::vec3 randomizeVec3InRadius(float radius) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::normal_distribution<float> dist(0.0f, radius);
		float x = dist(gen);
		float y = dist(gen);
		float z = dist(gen);
		return glm::vec3(x, y, z);
	}

    // Assuming these are globally accessible or passed in another way
    bool isMouseDragging;
    glm::vec2 initialMousePosition;

    glm::vec3 moveObjectInXAxis(GLFWwindow* window, const glm::vec3& objectPosition, const glm::vec3& cameraOrientation, const glm::vec3& cameraPosition) {
        if (!isMouseDragging) {
            return objectPosition;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 currentMousePosition(xpos, ypos);

        double deltaX = (currentMousePosition.x - initialMousePosition.x);
        double deltaY = (currentMousePosition.y - initialMousePosition.y);

        float distance = glm::distance(objectPosition, cameraPosition);
        float sensitivity = 0.0024f * (distance / 2);

        // Get the camera's actual forward direction.
        glm::vec3 actualCameraForward = glm::normalize(cameraOrientation);

        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        // Project camera's forward direction onto the horizontal plane (world XZ plane).
        glm::vec3 cameraForwardHorizontal = glm::normalize(glm::vec3(actualCameraForward.x, 0.0f, actualCameraForward.z));

        // Calculate the camera's horizontal right vector.
        glm::vec3 cameraRightHorizontal = glm::normalize(glm::cross(cameraForwardHorizontal, worldUp));

        // Handle edge cases where cameraForwardHorizontal is near zero (camera looking straight up or down).
        if (glm::length2(cameraForwardHorizontal) < 0.0001f) {
            // Camera is looking almost straight up or down.
            // Use a consistent world-aligned coordinate system
            cameraForwardHorizontal = glm::vec3(0.0f, 0.0f, -1.0f); // Always use world -Z as forward
            cameraRightHorizontal = glm::vec3(1.0f, 0.0f, 0.0f);    // Always use world +X as right
        }
        else {
            // Safety check for cameraRightHorizontal if cross product is tiny
            if (glm::length2(cameraRightHorizontal) < 0.0001f) {
                cameraRightHorizontal = glm::vec3(1.0f, 0.0f, 0.0f);
            }
        }

        glm::vec3 totalMovement = glm::vec3(0.0f);

        double effectiveDeltaY = -deltaY; // Default: mouse up (negative deltaY) moves forward

        // --- REVERSE DIRECTION FOR MOUSE Y WHEN CAMERA IS PARALLEL AND LOOKING UPWARDS ---
        // If the camera is looking upwards (positive Y component) while being roughly parallel,
        // reverse the movement direction for more intuitive control
        if (actualCameraForward.y > 0.1f) { // Camera is looking upwards (adjust threshold as needed)
            effectiveDeltaY = deltaY; // Reverse the sign of deltaY
        }

        // Determine which mouse movement axis is dominant and apply only that movement.
        if (glm::abs(deltaX) > glm::abs(deltaY)) {
            // Mouse X is dominant: Move object left/right relative to the camera's horizontal view.
            totalMovement = static_cast<float>(deltaX) * sensitivity * cameraRightHorizontal;
        }
        else {
            // Mouse Y is dominant: Move object forward/backward relative to the camera's horizontal view.
            totalMovement = static_cast<float>(effectiveDeltaY) * sensitivity * cameraForwardHorizontal;
        }

        glm::vec3 updatedObjectPosition = objectPosition + totalMovement;
        initialMousePosition = currentMousePosition; // Update initial mouse position for the next frame

        return updatedObjectPosition;
    }
    glm::vec3 moveObjectInZAxis(GLFWwindow* window, const glm::vec3& objectPosition, const glm::vec3& cameraOrientation, const glm::vec3& cameraPosition) {
        if (!isMouseDragging) {
            return objectPosition;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 currentMousePosition(xpos, ypos);

        double deltaX = (currentMousePosition.x - initialMousePosition.x);
        double deltaY = (currentMousePosition.y - initialMousePosition.y);

        float distance = glm::distance(objectPosition, cameraPosition);
        float sensitivity = 0.0024f * (distance / 2);

        // Get the camera's actual forward direction.
        glm::vec3 actualCameraForward = glm::normalize(cameraOrientation);

        glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f); // World X-axis for reference

        // Project camera's forward direction onto the vertical plane (world YZ plane).
        glm::vec3 cameraForwardVertical = glm::normalize(glm::vec3(0.0f, actualCameraForward.y, actualCameraForward.z));

        // Calculate the camera's vertical right vector (which is the Y-axis in YZ plane).
        glm::vec3 cameraRightVertical = glm::normalize(glm::cross(cameraForwardVertical, worldRight));

        // Handle edge cases where cameraForwardVertical is near zero (camera looking straight left or right).
        if (glm::length2(cameraForwardVertical) < 0.0001f) {
            // Camera is looking almost straight left or right.
            // Use a consistent world-aligned coordinate system
            cameraForwardVertical = glm::vec3(0.0f, 0.0f, -1.0f); // Always use world -Z as forward
            cameraRightVertical = glm::vec3(0.0f, 1.0f, 0.0f);    // Always use world +Y as right
        }
        else {
            // Safety check for cameraRightVertical if cross product is tiny
            if (glm::length2(cameraRightVertical) < 0.0001f) {
                cameraRightVertical = glm::vec3(0.0f, 1.0f, 0.0f);
            }
        }

        glm::vec3 totalMovement = glm::vec3(0.0f);

        double effectiveDeltaY = -deltaY; // Default: mouse up (negative deltaY) moves forward

        // --- REVERSE DIRECTION FOR MOUSE Y WHEN CAMERA IS LOOKING UPWARDS ---
        if (actualCameraForward.y > 0.1f) { // Camera is looking upwards
            effectiveDeltaY = deltaY; // Reverse the sign of deltaY
        }

        double effectiveDeltaX = deltaX; // Default mouse X direction

        // --- REVERSE DIRECTION FOR MOUSE X WHEN CAMERA IS LOOKING FROM CERTAIN SIDES ---
        // If the camera's X component is negative (looking from the left side), reverse mouse X
        if (actualCameraForward.x < -0.1f) { // Camera is looking from the left side
            effectiveDeltaX = -deltaX; // Reverse the sign of deltaX
        }

        // Determine which mouse movement axis is dominant and apply only that movement.
        if (glm::abs(deltaX) > glm::abs(deltaY)) {
            // Mouse X is dominant: Move object up/down relative to the camera's vertical view.
            totalMovement = static_cast<float>(effectiveDeltaX) * sensitivity * cameraRightVertical;
        }
        else {
            // Mouse Y is dominant: Move object forward/backward in Z relative to the camera's vertical view.
            totalMovement = static_cast<float>(effectiveDeltaY) * sensitivity * cameraForwardVertical;
        }

        glm::vec3 updatedObjectPosition = objectPosition + totalMovement;
        initialMousePosition = currentMousePosition; // Update initial mouse position for the next frame

        return updatedObjectPosition;
    }


    glm::vec3 moveObjectInYAxis(GLFWwindow* window, const glm::vec3& objectPosition, const glm::vec3& cameraOrientation, const glm::vec3& cameraPosition) {
        if (!isMouseDragging) {
            return objectPosition;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 currentMousePosition(xpos, ypos);

        double deltaX = (currentMousePosition.x - initialMousePosition.x);
        double deltaY = (currentMousePosition.y - initialMousePosition.y);

        float distance = glm::distance(objectPosition, cameraPosition);
        float sensitivity = 0.0024f * (distance / 2);

        glm::vec3 totalMovement = glm::vec3(0.0f);
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        // For Y-axis movement, we always move along the world Y axis (up/down)
        // Determine which mouse movement axis is dominant
        if (glm::abs(deltaX) > glm::abs(deltaY)) {
            // Mouse X is dominant: Move object up/down based on X movement
            totalMovement = static_cast<float>(deltaX) * sensitivity * worldUp;
        }
        else {
            // Mouse Y is dominant: Move object up/down based on Y movement
            // Note: negative deltaY means mouse moved up, so we want object to move up (positive Y)
            totalMovement = static_cast<float>(-deltaY) * sensitivity * worldUp;
        }

        glm::vec3 updatedObjectPosition = objectPosition + totalMovement;
        initialMousePosition = currentMousePosition;

        return updatedObjectPosition;
    }
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                initialMousePosition = glm::vec2(xpos, ypos);
                isMouseDragging = true;
            }
            else if (action == GLFW_RELEASE) {

                isMouseDragging = false;

            }
        }
    }
};