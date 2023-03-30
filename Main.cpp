#include"Object.h"
#include"Math.h"

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

glm::vec2 ratio(16, 9);

const unsigned int width = 90 * ratio.x;
const unsigned int height = 90 * ratio.y;



Vertex vertices[] =
{
	Vertex{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, -1.0f)},
	Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, -1.0f)},
	Vertex{glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};


const char* vertexShaderSource = 
"#version 330 core \n"

"layout(location = 0) in vec3 aPos;\n"

"layout(location = 1) in vec3 aNormal;\n"

"layout(location = 2) in vec3 aColor;\n"

"layout(location = 3) in vec2 aTex;\n"

"out vec3 crntPos;\n"

"out vec3 Normal;\n"

"out vec3 color;\n"

"out vec2 texCoord;\n"

"uniform mat4 camMatrix;\n"

"uniform mat4 model;\n"

"void main()\n"
"{ \n"
"	crntPos = vec3(model * vec4(aPos, 1.0f));\n"
"	Normal = aNormal;\n"
"	color = aColor;\n"
"	texCoord = aTex;\n"
"	gl_Position = camMatrix * vec4(crntPos, 1.0);\n"
"}\n";


const char* fragmentShaderSource =
"#version 330 core \n"

"out vec4 FragColor;\n"

"in vec3 crntPos;\n"

"in vec2 texCoord;\n"

"uniform sampler2D diffuse0;\n"

"vec4 lightColor = vec4(1, 1, 1, 1);\n"

"vec4 ambientLight()\n"
"{ \n"
"	float ambient = 1.0f;\n"
"	return texture(diffuse0, texCoord) * ambient * lightColor;\n"
"}\n"

"void main()\n"
"{ \n"
"	if (texture(diffuse0, texCoord).a < 0.1)\n"
"		discard;\n"

"	FragColor = ambientLight();\n"
"}\n";



float Lerp(float from, float to, float time)
{
	return from + time * (to - from);
}

int main()
{
	float scale = 0.5f;

	for (int i = 0; i < 4; i++) {
		vertices[i].position.x *= scale;
		vertices[i].position.y *= scale;
		vertices[i].position.z *= scale;
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
	

	Object box(verts, ind);

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 80.0f));
	float rotateSpeed = 1;

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
		box.Draw(window, shaderProgram, camera, 0 ,0, 20, 20, Deg((crntTime * 250 * rotateSpeed)), glm::vec3(0, 1, 0), width, height, ratio);

		ImGui::Begin("HELLO 1");
		{

			ImGui::InputFloat("Normal speed", &rotateSpeed, 0.3f, 1, "%.3f", 0);
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