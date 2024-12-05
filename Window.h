#ifndef OV_WINDOW_CLASS_H
#define OV_WINDOW_CLASS_H

#include <iostream>
#include <stb/stb_image.h>
#include "glfw3.h"
#include "glad/glad.h"
#include "InputSystem.h"
#include "EventManager.h"

class Window;

struct EventWindowResize
{
	Window* window;
	int width, height;
};

class Window
{
public:
	int width = 1280;
	int height = 800;
	int v_width = 1280;
	int v_height = 800;
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
		glfwSetWindowUserPointer(window, this);

		EventManager::addEvent<EventWindowResize>();

		EventManager::addCallback<EventWindowResize>(
			[&](const EventWindowResize* resizeEvent)
			{
				std::cout << "The new window size is: " << resizeEvent->width << " : " << resizeEvent->height << "\n";
			}
		);

		glfwSetWindowSizeCallback(window,
			[](GLFWwindow* window, int newWidth, int newHeight)
			{
				Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));
				EventManager::callCallback<EventWindowResize>({windowObject, newWidth, newHeight});

				// Update width and height in the Window class
				windowObject->width = newWidth;
				windowObject->height = newHeight;

				glViewport(0,0, newWidth, newHeight);
			}
		);


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

		//EventManager::addEvent<EventWindowResize>();

		inputSystem = std::make_unique<InputSystem>(window);
	}
	void setVieportSize(int w, int h)
	{
		v_width = w;
		v_height = h;
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

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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


	~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	GLFWwindow* getWindow()
	{
		return window;
	}

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