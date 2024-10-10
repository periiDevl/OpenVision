#ifndef OV_WINDOW_CLASS_H
#define OV_WINDOW_CLASS_H
#include <iostream>
#include "Camera.h"
#include "glfw3.h"
#include "glad/glad.h"
#include <stb/stb_image.h>
class OvWindow
{
public:
	int width = 800;
	int height = 800;
	void init()
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
		glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetScrollCallback(window, scroll_callback);

		//Load the OpenVision Icon.
		int wid, hei;
		int channels;
		unsigned char* pixels = stbi_load("OpenVisionIcon.png", &wid, &hei, &channels, 4);

		GLFWimage images[1];
		images[0].width = wid;
		images[0].height = hei;
		images[0].pixels = pixels;
		glfwSetWindowIcon(window, 1, images);

		gladLoadGL();
		glViewport(0, 0, width, height);
	}

	void update() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	bool windowRunning() {
		return !glfwWindowShouldClose(window);
	}

	void close() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

private:
	GLFWwindow* window;
};

#endif