#include"Object.h"
#include"Math.h"
#include"Settings.h"
#include"Console.h"
#include"IMGUITheme.h"
#include"ScriptsFunctions.h"
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include"PhysicsWorld.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Console con;
Scripting scr;

double scroll_offset = 45.0;

Camera camera(width, height, glm::vec3(0.0f, 0.0f, 80.0f));

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static double lastX = xpos;
	static double lastY = ypos;

	// Calculate the change in mouse position
	double deltaX = xpos - lastX;
	double deltaY = ypos - lastY;

	// Update last position
	lastX = xpos;
	lastY = ypos;

	// Calculate normalized device coordinates (NDC) of mouse position
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float ndcMouseX = (float)xpos / (float)width * 2.0f - 1.0f;
	float ndcMouseY = (float)ypos / (float)height * 2.0f - 1.0f;
	ndcMouseX *= rattio.x * 4;
	ndcMouseY *= rattio.y * 4;

	// Move 2D coordinate based on change in mouse position
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		camera.Position.x -= deltaX * 0.1f;
		camera.Position.y += deltaY * 0.1f; // invert Y axis
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll_offset += yoffset;
	printf("Scrolled by %f units\n", scroll_offset);
}

int main()
{
	PhysicsBody body1 = PhysicsBody(vec2(-1.5f, 2.0f), 0, vec2(0.3f, 0.5f), 5, 2, 3, 0.7f, 1, false);
	body1.SetVelocity(vec2(1.5f, 0.0f));


	PhysicsWorld world(vec3(0, -5.0f, 0), 10);
	world.AddBody(&body1);
	
	std::vector<std::string> py_files;

	const std::filesystem::path directory_path = std::filesystem::current_path();

	


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
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//load Icon
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

	GLuint unlitFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(unlitFragmentShader, 1, &UnlitFragment, NULL);
	glCompileShader(unlitFragmentShader);

	GLuint unlitProgram = glCreateProgram();
	glAttachShader(unlitProgram, vertexShader);
	glAttachShader(unlitProgram, unlitFragmentShader);
	glLinkProgram(unlitProgram);

	

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	


	float ndcMouseX;
	float ndcMouseY;
	double mouseX;
	double mouseY;
	double beforeMouseX;
	double beforeMouseY;
	bool run = false;
	bool startCompiling = false;


	
	
	std::vector<Texture> textures = { Texture("epicphoto.jpg", "diffuse", 0), Texture("itay.png", "diffuse", 0), Texture("texas.png", "diffuse", 0), Texture("ohio.png", "diffuse", 0),
		Texture("US.png", "diffuse", 0), Texture("flops.jpeg", "diffuse", 0) };
	


	std::vector<Object> sceneObjects;

	Object sceneobkj = Object(verts, ind);
	sceneobkj.tex = textures[2];
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	int selectedObject = 0;
	
	char addedfile[256] = "";

	const float fixed_timestep = 1.0f / 60.0;
	DefaultTheme();

	con.log("Hello world!");

	while (!glfwWindowShouldClose(window))
	{
		
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			run = false;
		}
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
		if (run) {
			scr.Load("ov.rtsm", sceneObjects, textures);
		}

		
		
		ImGui::Begin("Execute");
		if (ImGui::Button("run"))
		{
			if (run == false) {
				run = true;
			}
			else if (run == true)
			{
				run = false;
			}
		}
		ImGui::End();
		
		con.Draw();

		if (!run) {
			if (!startCompiling) {
				std::system("taskkill /F /IM python.exe");




				startCompiling = true;
			}

			ImGui::Begin("Assets");
			{
				if (ImGui::Button("Add Texture"))
				{
					
					textures.push_back(Texture(addedfile, "diffuse", 0));
				}

				ImGui::InputText("Path : ", addedfile, sizeof(addedfile));

				for (size_t k = 0; k < textures.size(); k++)
				{
					ImGui::Separator();


					if (ImGui::Selectable(("Bind : " + std::string(textures[k].ImageFile)).c_str())) {
						sceneObjects[selectedObject].tex = textures[k];

						con.log(("File : " + std::string(textures[k].ImageFile) + "Binded To : " + std::to_string(selectedObject)).c_str());

					}


					ImGui::Separator();
				}
			}

			ImGui::Begin("Object Inspector");

			if (ImGui::Button("Add object"))
			{

				sceneObjects.push_back(Object(verts, ind));

			}
			{
				for (size_t i = 0; i < sceneObjects.size(); i++)
				{
					if (sceneObjects[i].selected)
					{

						selectedObject = i;
						con.log(("User Select [" + std::to_string(selectedObject) + "]").c_str());

					}

					if (sceneObjects[i].deleted == false) {

						if (ImGui::CollapsingHeader(("Vision Object" + std::to_string(i)).c_str())) {
							if (ImGui::Button(("Delete Object##" + std::to_string(i)).c_str()))
							{
								sceneObjects[i].deleted = true;

							}


							ImGui::Columns(2, nullptr, true);

							ImGui::InputFloat(("Position X##" + std::to_string(i)).c_str(), &sceneObjects[i].calculatedPosition.x, 0.3f, 1, "%.3f", 0);
							ImGui::NextColumn();
							ImGui::InputFloat(("Position Y##" + std::to_string(i)).c_str(), &sceneObjects[i].calculatedPosition.y, 0.3f, 1, "%.3f", 0);

							ImGui::Columns(1, nullptr, true);
							ImGui::Columns(2, nullptr, true);
							ImGui::InputFloat(("Scale X##" + std::to_string(i)).c_str(), &sceneObjects[i].ScaleX, 0.3f, 1, "%.3f", 0);
							ImGui::NextColumn();

							ImGui::InputFloat(("Scale Y##" + std::to_string(i)).c_str(), &sceneObjects[i].ScaleY, 0.3f, 1, "%.3f", 0);

							ImGui::Columns(1, nullptr, true);
							ImGui::InputFloat(("Angle ##" + std::to_string(i)).c_str(), &sceneObjects[i].angle, 0.3f, 1, "%.3f", 0);







						}
						ImGui::Separator();


						glfwGetCursorPos(window, &mouseX, &mouseY);

						ndcMouseX = (float)mouseX / (float)width * 2.0f - 1.0f;
						ndcMouseY = (float)mouseY / (float)height * 2.0f - 1.0f;
						ndcMouseX *= rattio.x * 4;
						ndcMouseY *= rattio.y * 4;
						if ((sceneObjects[i].calculatedPosition.x - sceneObjects[i].ScaleX / 3) - camera.Position.x < ndcMouseX &&
							(sceneObjects[i].calculatedPosition.x + sceneObjects[i].ScaleX / 3) + camera.Position.x > ndcMouseX &&
							(sceneObjects[i].calculatedPosition.y + sceneObjects[i].ScaleY / 3) - camera.Position.y > ndcMouseY &&
							(sceneObjects[i].calculatedPosition.y - sceneObjects[i].ScaleY / 3) + camera.Position.y < ndcMouseY
							&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
						{
							if (!sceneObjects[i].selected) {
								beforeMouseX = ndcMouseX;
								beforeMouseY = ndcMouseY;
							}
							else {
								float dx = ndcMouseX - beforeMouseX;
								float dy = ndcMouseY - beforeMouseY;

								sceneObjects[i].calculatedPosition.x += dx;
								sceneObjects[i].calculatedPosition.y += dy;

								beforeMouseX = ndcMouseX;
								beforeMouseY = ndcMouseY;
							}

							sceneObjects[i].selected = true;
						}
						else {
							sceneObjects[i].selected = false;
						}

						

						sceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), width, height, rattio);
						glUseProgram(unlitProgram);
						glUniform4f(glGetUniformLocation(unlitProgram, "color"), 1.00, 0.56, 0.13, 1);

						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glLineWidth(3.0f);
						sceneObjects[selectedObject].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), width, height, rattio);
						glLineWidth(0.0f);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

					}
				}
			}
		}
		sceneobkj.DrawTMP(window, shaderProgram, camera, glm::vec2(body1.GetPosition().x, body1.GetPosition().y));
		if (run) {
			if (startCompiling)
			{
				py_files.clear();
				for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
					if (entry.is_regular_file() && entry.path().extension() == ".py" && entry.path().filename() != "ov.py") {
						py_files.push_back(entry.path().filename().string());
					}
				}

				for (const auto& pyfile : py_files) {
					std::cout << pyfile << std::endl;
					std::string command = "start /B python " + pyfile;
					std::system(command.c_str());
				}

				startCompiling = false;
			}
			for (size_t i = 0; i < sceneObjects.size(); i++)
			{
				glLineWidth(0.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glUseProgram(unlitProgram);
				glUniform4f(glGetUniformLocation(unlitProgram, "color"), sceneObjects[i].OutlineColor.x, sceneObjects[i].OutlineColor.y, sceneObjects[i].OutlineColor.z, 1);
				glLineWidth(sceneObjects[i].outlineWidth);
				if (sceneObjects[i].outlineWidth > 0) {
					sceneObjects[i].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), width, height, rattio);
				}
				glLineWidth(0.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				sceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), width, height, rattio);
			}
			
		}
		
		world.Step(1.0f / 60);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSetCursorPosCallback(window, mouse_callback);
		scr.Relase("ov.rtsm");
	}


	
	scr.Relase("ov.rtsm");
	remove("ov.rtsm");

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