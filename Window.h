#ifndef OV_WINDOW_CLASS_H
#define OV_WINDOW_CLASS_H

#include <iostream>
#include "Camera.h"
#include <stb/stb_image.h>
#include "glfw3.h"
#include "glad/glad.h"
#include "InputSystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

class Window
{
public:
	int width = 1900;
	int height = 800;
	
	Window()
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_SAMPLES, msaa);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		window = glfwCreateWindow(width, height, "Loading...", NULL, NULL);
		
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		
		glfwMakeContextCurrent(window);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetScrollCallback(window, scroll_callback);

		// disable VSync
		glfwSwapInterval(0);

		//Load the OpenVision Icon.
		int wid, hei;
		int channels;
		unsigned char* pixels = stbi_load("OpenVisionIcon.png", &wid, &hei, &channels, 4);

		GLFWimage image;
		image.width = wid;
		image.height = hei;
		image.pixels = pixels;
		glfwSetWindowIcon(window, 1, &image);

		gladLoadGL();
		glViewport(0, 0, width, height);

		inputSystem = std::make_unique<InputSystem>(window); 
	}
	
	void clear()
	{

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}
	
	void setTitle(const std::string& title)
	{
		glfwSetWindowTitle(window, title.c_str());
	}

	void update() 
	{
		glfwSwapBuffers(window);

		inputSystem->update();

		glfwPollEvents();
	}

	bool windowRunning() 
	{
		return !glfwWindowShouldClose(window);
	}

	glm::vec2 mouseAsWorldPosition(Camera& cam)
	{
		glm::vec2 mousePos = InputSystem::getMousePosition();

		float ndcX = (2.0f * mousePos.x) / width - 1.0f;
		float ndcY = 1.0f - (2.0f * mousePos.y) / height; // Y is inverted in GLFW

		// Step 3: Transform NDC to world coordinates using the inverse projection matrix
		// Set up the orthographic projection matrix, as before
		float orthoSize = 1.0f; // Example orthographic size
		float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		// Invert the orthographic projection matrix to convert NDC to world coordinates
		glm::mat4 inverseProjection = glm::inverse(cam.cameraMatrix);
		glm::vec4 ndcPosition = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
		glm::vec4 worldPosition = inverseProjection * ndcPosition;

		// Extract the X and Y world coordinates from the result
		glm::vec2 worldCoords = glm::vec2(worldPosition.x, worldPosition.y);

		return worldCoords;
	}

	~Window() 
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	GLFWwindow* getWindow()
	{
		return window;
	}

private:
	
	GLFWwindow* window;
	std::unique_ptr<InputSystem> inputSystem;

};

#endif