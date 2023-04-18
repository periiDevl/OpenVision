#include"Object.h"
#include"Math.h"
#include <iostream>

#include "PhysicsWorld.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int width = 90 * 16;
const unsigned int height = 90 * 9;



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
	
	Texture mario = Texture("mario.png", "diffuse", 0);

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


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 5.0f));

	PhysicsBody body1 = PhysicsBody(vec2(-1.5f, 2.0f), 0, vec2(0.3f,0.5f), 5, 2, 3, 0.7f, 1, false);
	body1.SetVelocity(vec2(1.5f, 0.0f));

	PhysicsBody body2 = PhysicsBody(vec2(0.0f, 0.0f), 0, vec2(5, 0.5f), 5, 2, 3, 0.0f, 1, true);
	
	PhysicsWorld world(vec3(0, -5.0f, 0), 10);
	world.AddBody(&body1);
	world.AddBody(&body2);

	const float fixed_timestep = 1.0f / 300.0;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		camera.updateMatrix(30.0f, 0.1f, 100.0f);


		
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= fixed_timestep) {
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string newTitle = "| OpenVision | periiDev & ItayMadeIt [ " + FPS + " FPS ]";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
			
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);


			float D = glfwGetKey(window, GLFW_KEY_D) ? 1 : 0;
			float A = glfwGetKey(window, GLFW_KEY_A) ? 1 : 0;
			float horizontal = D - A;


			float speed = 5;


			body1.SetVelocity(vec2(horizontal * speed, body1.GetVelocity().y));
			
			if (glfwGetKey(window, GLFW_KEY_SPACE))
				body1.SetVelocity(vec2(horizontal * speed, 1.5f));
			

			world.Step(fixed_timestep);
		}

		texas.Bind();
		box.Draw(shaderProgram, camera, 0, 0, 6, 4, radians(90.0f), glm::vec3(0, 0, 1));

		mario.Bind();
		box.Draw(shaderProgram, camera, body1.GetPosition().x, body1.GetPosition().y, body1.GetScale().x*2, body1.GetScale().y*2, radians(body1.GetRotation()), glm::vec3(0, 0, 1));
		
		perii.Bind();
		box.Draw(shaderProgram, camera, body2.GetPosition().x, body2.GetPosition().y, body2.GetScale().x*2, body2.GetScale().y*2, radians(body2.GetRotation()), glm::vec3(-1, -1, -1));

		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.Position = vec3(body1.GetPosition().x, camera.Position.y, camera.Position.z);

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