#ifndef OV_WINDOW_CLASS_H
#define OV_WINDOW_CLASS_H

#include <iostream>
#include "Camera2D.h"
#include <stb/stb_image.h>
#include "glfw3.h"
#include "glad/glad.h"
#include "InputSystem.h"
/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}*/


class Window
{
public:
	int width = 1280;
	int height = 800;

	Window(Camera2D& camera)
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
		Window::s_camera = &camera;
		glfwSetWindowUserPointer(window, this);

		glfwSetWindowSizeCallback(window,
			[](GLFWwindow* window, int newWidth, int newHeight)
			{
				// Update width and height in the Window class
				Window::s_camera->setAspectRatio(newWidth, newHeight);
				Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				win->width = newWidth;
				win->height = newHeight;

				glViewport(0, 0, newWidth, newHeight);


			});


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
	// Delete copy constructor and copy assignment operator
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	// Implement move constructor and move assignment operator (if needed)
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
	void resizeFramebuffer(unsigned int framebufferTexture, unsigned int RBO, int newWidth, int newHeight) {
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);
	}
	static void resizeFramebufferUint(GLuint& gluint, GLuint& textureColorBuffer, int newWidth, int newHeight) {
		glBindFramebuffer(GL_FRAMEBUFFER, gluint);

		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	glm::vec2 mouseAsWorldPosition(Camera2D& cam)
	{
		glm::vec2 mousePos = InputSystem::getMousePosition();

		cam.updateMatrix(0.1, 100);

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

	static Camera2D* s_camera;

	enum class Cursors
	{
		ArrowCursor = GLFW_ARROW_CURSOR,       // Standard arrow cursor
		IBeamCursor = GLFW_IBEAM_CURSOR,       // Text input (I-beam) cursor
		CrosshairCursor = GLFW_CROSSHAIR_CURSOR,   // Crosshair cursor
		HandCursor = GLFW_HAND_CURSOR,        // Hand cursor for clickable elements
		HResizeCursor = GLFW_HRESIZE_CURSOR,     // Horizontal resize cursor
		VResizeCursor = GLFW_VRESIZE_CURSOR      // Vertical resize cursor
	};
	void setCursor(Cursors cursorType) {
		// Create the cursor from the enum type
		GLFWcursor* cursor = glfwCreateStandardCursor(static_cast<int>(cursorType));

		// Set the cursor for the window
		glfwSetCursor(window, cursor);
	}
private:


	GLFWwindow* window;
	std::unique_ptr<InputSystem> inputSystem;

};

#endif