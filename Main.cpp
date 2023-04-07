#include"Object.h"
#include"Math.h"
#include"Settings.h"
#include"Console.h"
#include"IMGUITheme.h"

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Console con;

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
	



	
	
	
	Texture textures[] = { Texture("epicphoto.jpg", "diffuse", 0), Texture("itay.png", "diffuse", 0), Texture("texas.png", "diffuse", 0), Texture("ohio.png", "diffuse", 0),
	Texture("US.png", "diffuse", 0), Texture("flops.jpeg", "diffuse", 0) };

	
	Object sceneObjects[] = { Object(verts, ind), Object(verts, ind) };

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	int selectedObject = 0;
	

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 80.0f));
	const float fixed_timestep = 1.0f / 60.0;
	DefaultTheme();

	con.log("Hello world!");

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



		ImGui::Begin("Assets");
		{
			for (size_t k = 0; k < sizeof(textures) / sizeof(textures[0]); k++)
			{
				ImGui::Separator();


				if (ImGui::Selectable(("Bind : " + std::string(textures[k].ImageFile)).c_str())) {
					sceneObjects[selectedObject].tex = textures[k];

					con.log(("File : " + std::string(textures[k].ImageFile) + "Binded To : " + std::to_string(selectedObject)).c_str());

				}


				ImGui::Separator();
			}
		}
		

		
		con.Draw();
		ImGui::Begin("Object Inspector");
		{
			for (size_t i = 0; i < sizeof(sceneObjects) / sizeof(sceneObjects[0]); i++)
			{
				if (sceneObjects[i].selected)
				{
					selectedObject = i;
					con.log(("User Select [" + std::to_string(selectedObject) + "]").c_str());
				}

				ImGui::Separator();
				ImGui::Separator();

				if (ImGui::Button(("Remove object##" + std::to_string(i)).c_str()))
				{
					//I want to store it to the save system and remove the object afterward...
					sceneObjects[i].ScaleX = 0;
					sceneObjects[i].ScaleY = 0;
				}


				ImGui::Columns(2, nullptr, true);

				ImGui::InputFloat(("Position X##" + std::to_string(i)).c_str(), &sceneObjects[i].calculatedPosition.x, 0.3f, 1, "%.3f", 0);
				ImGui::NextColumn();
				ImGui::InputFloat(("Position Y##" + std::to_string(i)).c_str(), &sceneObjects[i].calculatedPosition.y, 0.3f, 1, "%.3f", 0);

				ImGui::Columns(1, nullptr, true);
				ImGui::Separator();
				ImGui::Columns(2, nullptr, true);
				ImGui::InputFloat(("Scale X##" + std::to_string(i)).c_str(), &sceneObjects[i].ScaleX, 0.3f, 1, "%.3f", 0);
				ImGui::NextColumn();

				ImGui::InputFloat(("Scale Y##" + std::to_string(i)).c_str(), &sceneObjects[i].ScaleY, 0.3f, 1, "%.3f", 0);

				ImGui::Columns(1, nullptr, true);
				ImGui::InputFloat(("Angle ##" + std::to_string(i)).c_str(), &sceneObjects[i].angle, 0.3f, 1, "%.3f", 0);

				
				if (ImGui::CollapsingHeader("Quick-Actions")) {
					for (size_t k = 0; k < sizeof(textures) / sizeof(textures[0]); k++)
					{
						ImGui::Separator();


						if (ImGui::Selectable(("Bind : " + std::string(textures[k].ImageFile) + std::to_string(i)).c_str()))
						{
							sceneObjects[i].tex = textures[k];
						}

						ImGui::Separator();
					}
				}





				ImGui::Separator();
				ImGui::Separator();


				sceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), width, height, ratio);
			}
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