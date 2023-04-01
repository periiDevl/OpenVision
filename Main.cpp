#include"Object.h"
#include"Math.h"
#include"Settings.h"



#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);




int main()
{
	for (int i = 0; i < 4; i++) {
		vertices[i].position.x *= GlobalWorldScale;
		vertices[i].position.y *= GlobalWorldScale;
		vertices[i].position.z *= GlobalWorldScale;
	}

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(width, height, "Loading...", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	gladLoadGL();
	glViewport(0, 0, width, height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);



	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	

	Texture perii = Texture("epicphoto.jpg", "diffuse", 0);

	Texture itay = Texture("itay.png", "diffuse", 0);

	Texture texas = Texture("texas.png", "diffuse", 0);
	
	Texture ohio = Texture("ohio.png", "diffuse", 0);
	
	Texture us = Texture("US.png", "diffuse", 0);

	Texture flops = Texture("flops.jpeg", "diffuse", 0);
	


	Object sceneObjects[] = { Object(verts, ind)};

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;



	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 80.0f));
	const float fixed_timestep = 1.0f / 60.0;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (timeDiff >= fixed_timestep) {
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string newTitle = "OpenVision - periidev & itaymadeit ~" + FPS + "FPS";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
		}



		

		flops.Bind();
		

		for (size_t i = 0; i < sizeof(sceneObjects) / sizeof(sceneObjects[0]); i++)
		{

			float posX, posY;
			float scaleX, scaleY;
			ImGui::Begin("Object Inspector" + i);
			{
				ImGui::InputFloat("Position X", &posX, 0.3f, 1, "%.3f", 0);
				ImGui::InputFloat("Position Y", &posY, 0.3f, 1, "%.3f", 0);

				ImGui::InputFloat("Scale X", &scaleX, 0.3f, 1, "%.3f", 0);
				ImGui::InputFloat("Scale Y", &scaleY, 0.3f, 1, "%.3f", 0);
			}

			sceneObjects[i].PositionX = posX;
			sceneObjects[i].PositionY = posY;
			sceneObjects[i].ScaleX = scaleX;
			sceneObjects[i].ScaleY = scaleY;

			sceneObjects[i].Draw(window, shaderProgram, camera, 0, glm::vec3(0, 0, 1), width, height, ratio);
		}


		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}