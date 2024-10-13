#ifndef OV_WINDOW_CLASS_H
#define OV_WINDOW_CLASS_H
#include <iostream>
#include "Camera.h"
#include <stb/stb_image.h>
#include "glfw3.h"
#include "glad/glad.h"
#include "InputSystem.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
class Window
{
public:
	int width = 1200;
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

#endif