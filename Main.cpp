#include"Object.h"
#include"Math.h"
#include"Settings.h"
#include"IMGUITheme.h"
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stack>
#include <variant>
#include <filesystem>
#include"PhysicsWorld.h"
#include <thread>
#include"Console.h"
#include"OVscriptHandaling.h"
#include"Presave.h"
#include"Script.h"
#include"AddedScript.h"
#include "SaveSystem.h"
std::vector<Object> sceneObjects;
std::vector<Object> PresceneObjects;
SaveSystem SavingSystem;


struct DeleteObject {
	Object object;

	DeleteObject(const Object& obj) : object(obj) {}
};
struct AddObject {
	string name;

	AddObject(const string& n) : name(n) {}

};
struct ModifyGuiBool {
	bool wasValue;
	bool* valueInMemory;

	ModifyGuiBool(const bool v, bool* memory) : wasValue(v), valueInMemory(memory) {}
};
struct ModifyGuiInt {
	int wasValue;
	int* valueInMemory;

	ModifyGuiInt(const int v, int* memory) : wasValue(v) , valueInMemory(memory) {}

};
struct ModifyGuiFloat {
	float wasValue;
	float* valueInMemory;

	ModifyGuiFloat(const float v, float* memory) : wasValue(v), valueInMemory(memory) {};
};
struct ModifyGuiString {
	std::string wasValue;
	std::string* valueInMemory;

	ModifyGuiString(const std::string v, std::string* memory) : wasValue(v), valueInMemory(memory) {};
};

using Action = std::variant<AddObject, DeleteObject, ModifyGuiBool, ModifyGuiInt, ModifyGuiFloat, ModifyGuiString>;

std::stack<Action> undoStack;

std::unordered_map<std::string, bool> focusStates;
std::unordered_map<std::string, bool> previousValuesB;

bool InputBoolWithEndFocus(const char* label, bool* value)
{
	bool valueChanged = ImGui::Checkbox(label, value);
	bool isItemActive = ImGui::IsItemActive();

	bool& isFocused = focusStates[std::string(label)];  // Retrieve or create the focus state for this label

	if (isItemActive)
	{
		if (!isFocused)
		{
			// Store the previous value only if focus was acquired for the first time
			isFocused = true;
			previousValuesB[std::string(label)] = *value;
		}
	}
	else if (isFocused && previousValuesB[std::string(label)] != *value)
	{
		ModifyGuiBool action(previousValuesB[std::string(label)], value);
		isFocused = false;
		undoStack.push(action);
	}

	return valueChanged;
}
std::unordered_map<std::string, int> previousValuesI;

bool InputIntWithEndFocus(const char* label, int* value, int step = 1, int stepFast = 100, ImGuiInputTextFlags flags = 0)
{
	bool valueChanged = ImGui::InputInt(label, value, step, stepFast, flags);
	bool isItemActive = ImGui::IsItemActive();

	bool& isFocused = focusStates[std::string(label)];  // Retrieve or create the focus state for this label

	if (isItemActive)
	{
		if (!isFocused)
		{
			// Store the previous value only if focus was acquired for the first time
			isFocused = true;
			previousValuesI[std::string(label)] = *value;
		}
	}
	else if (isFocused && previousValuesI[std::string(label)] != *value)
	{
		ModifyGuiInt action(previousValuesI[std::string(label)], value);
		isFocused = false;
		undoStack.push(action);
	}

	return valueChanged;
}

std::unordered_map<std::string, float> previousValuesF;

bool InputFloatWithEndFocus(const char* label, float* value, float step = 0.0f, float stepFast = 0.0f, ImGuiInputTextFlags flags = 0)
{
	std::ostringstream formatStream;
	formatStream << "%" << step << "f";
	std::string formatString = formatStream.str();

	bool valueChanged = ImGui::InputFloat(label, value, step, stepFast, formatString.c_str(), flags);
	bool isItemActive = ImGui::IsItemActive();

	bool& isFocused = focusStates[std::string(label)];  // Retrieve or create the focus state for this label

	if (isItemActive)
	{
		if (!isFocused)
		{
			// Store the previous value only if focus was acquired for the first time
			isFocused = true;
			previousValuesF[std::string(label)] = *value;
		}
	}
	else if (isFocused && previousValuesF[std::string(label)] != *value)
	{
		ModifyGuiFloat action(previousValuesF[std::string(label)], value);
		isFocused = false;
		undoStack.push(action);
	}

	return valueChanged;
}

void ObjectCreator(GLFWwindow* window, Object obj) {
	static bool hasExecuted = false;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) && glfwGetKey(window, GLFW_KEY_D) && !hasExecuted) {
		PresceneObjects.push_back(Object(verts, ind));
		PresceneObjects[PresceneObjects.size() - 1].name = "obj Copyof (" + obj.name + ") id (" + to_string(PresceneObjects.size()) + ")";
		//PresceneObjects[PresceneObjects.size() - 1].position = obj.position;
		PresceneObjects[PresceneObjects.size() - 1].tex = obj.tex;
		PresceneObjects[PresceneObjects.size() - 1].texChar = obj.texChar;
		PresceneObjects[PresceneObjects.size() - 1].scale = obj.scale;
		PresceneObjects[PresceneObjects.size() - 1].angle = obj.angle;
		hasExecuted = true;
	}

	if (!glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || !glfwGetKey(window, GLFW_KEY_D)) {
		hasExecuted = false;
	}
}


void createFile(const char* filename) {
	std::ofstream file(filename);

	if (!file) {
		std::cerr << "Error creating file: " << filename << std::endl;
		return;
	}

	file.close();
}
void copyFile(string sourcePath, string destinationPath) {
	if (filesystem::exists(sourcePath)) {
		if (filesystem::exists(filesystem::path(destinationPath).parent_path())) {
			fstream source(sourcePath, ios::in | ios::binary);

			fstream destination(destinationPath, ios::out | ios::binary);

			char buffer[1024];
			while (source.read(buffer, sizeof(buffer))) {
				destination.write(buffer, source.gcount());
			}

			source.close();
			destination.close();
		}
		else {
			cout << "Destination directory does not exist!" << endl;
		}
	}
	else {
		cout << "Source file does not exist!" << endl;
	}
}

void createFolder(string folderName) {
	if (filesystem::exists(folderName)) {

		std::filesystem::remove_all("BuildGL");
	}
	
	filesystem::create_directory(folderName);
	
}

Script script;
AddedScript AddedScriptscr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Console con;

double scroll_offset = 45.0;

int PythonIndex = 0;




float rectangleVertices[] =
{
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

std::string executeCommandAndGetOutput(const char* command) {
	std::string outputFileName = "command_output.txt";
	std::string commandWithOutputRedirect = std::string(command) + " > \"" + outputFileName + "\"";

	std::system(commandWithOutputRedirect.c_str());

	std::ifstream outputFile(outputFileName);
	if (!outputFile.is_open()) {
		std::cerr << "Failed to open output file." << std::endl;
		return "";
	}

	std::stringstream output;
	output << outputFile.rdbuf();
	outputFile.close();

	std::remove(outputFileName.c_str());

	return output.str();
}
std::string getPythonLocationByLine(int line) {
	std::string whereCommand = "where python";
	std::string pythonLocations = executeCommandAndGetOutput(whereCommand.c_str());

	std::stringstream ss(pythonLocations);
	std::string location;
	std::vector<std::string> locations;

	while (std::getline(ss, location, '\n')) {
		locations.push_back(location);
	}

	if (line < 0)
		line = 0;
	if (line >= locations.size()) {
		line = locations.size() - 1;
	}

	return locations[line];
}
void rebuild(GLFWwindow* window, bool localPython) {
	std::cout << "python locations " << endl << executeCommandAndGetOutput("where python") << endl;
	std::cout << getPythonLocationByLine(PythonIndex) << endl;
	std::system((getPythonLocationByLine(PythonIndex) + std::string(" -m pip install watchdog")).c_str());
	std::string command = std::string("start /B python builder.py");

	if (!localPython) {
		std::filesystem::path pythonPath = getPythonLocationByLine(PythonIndex);
		std::string pythonPathStr = pythonPath.string();

		for (size_t i = 0; i < pythonPathStr.size(); i++) {
			if (pythonPathStr[i] == '\\') {
				pythonPathStr.insert(i, 1, '\\');
				i++;
			}
		}

		command = std::string("start /B python.exe") + " builder.py";
	}

	std::cout << "command: " << command << std::endl;


	std::system(command.c_str());

	std::chrono::seconds wait_time(1);
	std::this_thread::sleep_for(wait_time);
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll_offset += yoffset;

}

// A function for ImGui textinput

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventChar == '\r' || data->EventChar == '\n')
	{
		return 0;
	}
	return 1;
}

bool file_exists(const std::string& filename) {
	std::ifstream file(filename);
	return file.good();
}
void undoAction() {
	if (!undoStack.empty()) {
		Action previousAction = undoStack.top();
		undoStack.pop();

		if (const AddObject* addObjectAction = std::get_if<AddObject>(&previousAction)) {

			for (size_t i = 0; i < PresceneObjects.size(); i++)
			{
				if (PresceneObjects[i].name == addObjectAction->name) {

					PresceneObjects[i].deleted = true;
					SavingSystem.remove("OBJ" + std::to_string(i) + "_NAME");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_POS_X");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_POS_Y");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_SCA_X");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_SCA_Y");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_ANGLE");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_TEXTURE");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_LAYER");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_RUNDRAW");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_FRIC");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_STATIC");
					SavingSystem.remove("OBJ" + std::to_string(i) + "_TRIG");
					PresceneObjects.erase(PresceneObjects.begin() + i);
					sceneObjects.erase(sceneObjects.begin() + i);
					return;
				}
			}
		}
		else if (const DeleteObject* deleteObjectAction = std::get_if<DeleteObject>(&previousAction)) {

			PresceneObjects.push_back(deleteObjectAction->object);
		}
		else if (const ModifyGuiInt* modifyIntAction = std::get_if<ModifyGuiInt>(&previousAction)) {
			*modifyIntAction->valueInMemory = modifyIntAction->wasValue;
		}
		else if (const ModifyGuiFloat* modifyFloatAction = std::get_if<ModifyGuiFloat>(&previousAction)) {
			*modifyFloatAction->valueInMemory = modifyFloatAction->wasValue;
		}
		else if (const ModifyGuiString* modifyStringAction = std::get_if<ModifyGuiString>(&previousAction)) {
			*modifyStringAction->valueInMemory = modifyStringAction->wasValue;
		}
		else if (const ModifyGuiBool* modifyBoolAction = std::get_if<ModifyGuiBool>(&previousAction)) {
			*modifyBoolAction->valueInMemory = modifyBoolAction->wasValue;
		}
	}
}


int main()
{
	Presave<float> myData;

	myData = Presave < float >();
	myData.SafeOperation();
	
	bool vsync = myData.data[0];
	int msaa = myData.data[1];
	bool LocalPy = myData.data[5];
	PythonIndex = myData.data[6];
	bool DrawFramebuffer = myData.data[7];
	float VigRadius = myData.data[8];
	float VigSoftness = myData.data[9];
	float FXAA_SPAN_MAX = myData.data[10];
	float FXAA_REDUCE_MIN = myData.data[11];
	float FXAA_REDUCE_MUL = myData.data[12];
	float CMX = myData.data[13];
	float CMY = myData.data[14];
	bool build;

	

	std::vector<std::string> lines;

	std::ifstream inputFile("SCRIPTS.ov");
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open input file." << std::endl;
		return 1;
	}
	std::string line;
	while (std::getline(inputFile, line)) {
		lines.emplace_back(line);
	}
	inputFile.close();
	
	PhysicsWorld world(vec3(0, 55.0f, 0), 10);


	const std::filesystem::path directory_path = std::filesystem::current_path();



	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, msaa);
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


	GLuint vertexFrame = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexFrame, 1, &FrameBufferVert, NULL);
	glCompileShader(vertexFrame);
	 
	GLuint fragmentFrame = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentFrame, 1, &FrameBufferFrag, NULL);
	glCompileShader(fragmentFrame);

	GLuint FramebufferProgram = glCreateProgram();
	glAttachShader(FramebufferProgram, vertexFrame);
	glAttachShader(FramebufferProgram, fragmentFrame);
	glLinkProgram(FramebufferProgram);
	

	glUseProgram(FramebufferProgram);
	glUniform1i(glGetUniformLocation(FramebufferProgram, "screenTexture"), 0);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "radius"), VigRadius);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "softness"), VigSoftness);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "minEdgeContrast"), FXAA_REDUCE_MIN);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "subPixelAliasing"), FXAA_REDUCE_MUL);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "maximumEdgeDetection"), FXAA_SPAN_MAX);
	glUniform2f(glGetUniformLocation(FramebufferProgram, "resolution"), width, height);



	float ndcMouseX;
	float ndcMouseY;
	double mouseX;
	double mouseY;
	double beforeMouseX;
	double beforeMouseY;

	double beforeMouseXCam;
	double beforeMouseYCam;
	bool run = false;
	bool StartPhase = false;
	bool no_resize = true;
	bool no_move = true;
	bool mouseOverUI = false;


	std::vector<Texture> textures;
	std::filesystem::path dir_path = "Assets/";

	for (const auto& file : std::filesystem::directory_iterator(dir_path)) {
		std::string ext = file.path().extension().string();
		if (ext == ".jpg" || ext == ".png" || ext == ".jpeg") {
			std::string filepath = file.path().string();
			Texture tex(filepath.c_str());
			tex.ImageFile = tex.ImageFile = file.path().stem().string();
			tex.FullImageFile = filepath;
			textures.push_back(tex);
		}
	}


	SaveSystem SavingSystem;
	InputSystem InputHandler;

	SavingSystem.load("SCENE.ov");


	vec3 bg_rgb = SavingSystem.getVec3("BG_COLOR", vec3(0));
	float BackroundScreen[3];
	BackroundScreen[0] =bg_rgb.r;
	BackroundScreen[1] =bg_rgb.g;
	BackroundScreen[2] =bg_rgb.b;


	int amount = SavingSystem.getInt("OBJ_AMOUNT", 3);
	for (int i = 0; i < amount; i++) {
		float posx, posy, scalex, scaley, angle, layer, restitution, friction, velX, velY;
		bool runtimeDraw, isStatic, isTrigger;
		int phys_layer = 0;
		std::string name, texture;

		name = SavingSystem.getString("OBJ" + std::to_string(i) + "_NAME", std::to_string(i).c_str());
		posx    = SavingSystem.getFloat ("OBJ" + std::to_string(i) + "_POS_X"  , 0.0f);
		posy    = SavingSystem.getFloat ("OBJ" + std::to_string(i) + "_POS_Y"  , 0.0f);
		scalex  = SavingSystem.getFloat ("OBJ" + std::to_string(i) + "_SCA_X"  , 0.0f);
		scaley  = SavingSystem.getFloat ("OBJ" + std::to_string(i) + "_SCA_Y"  , 0.0f);
		angle   = SavingSystem.getFloat ("OBJ" + std::to_string(i) + "_ANGLE"  , 0.0f);
		texture = SavingSystem.getString("OBJ" + std::to_string(i) + "_TEXTURE", "");
		layer = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_LAYER", 0.0f);
		runtimeDraw = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_RUNDRAW", 0.0f);

		phys_layer = SavingSystem.getInt("OBJ" + std::to_string(i) + "_PHYSLAYER", 0);
		friction = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_FRIC", 0.5f);
		restitution = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_BOUNCE", 0.5f);
		isStatic = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_STATIC", 0.0f);
		isTrigger = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_TRIG", 0.0f);

		Object obj = Object(verts, ind);
		obj.name = name;
		obj.position->x = posx;
		obj.position->y = posy;
		obj.scenePosition.x = posx;
		obj.scenePosition.y = posy;
		obj.scale->x = scalex;
		obj.scale->y = scaley;
		obj.sceneScale.x = scalex;
		obj.sceneScale.y = scaley;
	   *obj.angle = angle;
		obj.texChar = texture;
		obj.layer = layer;
		obj.drawOnRuntime = runtimeDraw;

		obj.Body->layer = phys_layer;
		obj.Body->friction = friction;
		obj.Body->restitution = restitution;
		obj.Body->isStatic = isStatic;
		obj.Body->isTrigger = isTrigger;


		obj.tex = Texture((texture).c_str());
		PresceneObjects.push_back(obj);
	}

	Object blackbox = Object(verts, ind);
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	int selectedObject = 0;
	float fov = 45;
	char addedfile[256] = "";
	bool onpopupmenu = false;

	bool releasedUndo = true;



	const float fixed_timestep = 1.0f / 60.0;
	DefaultTheme();
	glEnable(GL_MULTISAMPLE);

	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i].scenePosition = *PresceneObjects[i].position;
		sceneObjects[i].sceneScale = *PresceneObjects[i].scale;
	}
	sceneObjects = PresceneObjects;
	glEnable(GL_DEPTH_TEST);


	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		build = file_exists("ov.ov");
		glUniform1f(glGetUniformLocation(FramebufferProgram, "minEdgeContrast"), FXAA_REDUCE_MIN);
		glUniform1f(glGetUniformLocation(FramebufferProgram, "subPixelAliasing"), FXAA_REDUCE_MUL);
		glUniform1f(glGetUniformLocation(FramebufferProgram, "maximumEdgeDetection"), FXAA_SPAN_MAX);
		glUniform1f(glGetUniformLocation(FramebufferProgram, "radius"), VigRadius);
		glUniform1f(glGetUniformLocation(FramebufferProgram, "softness"), VigSoftness);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) && !build)
		{
			run = false;
		}
		if (DrawFramebuffer && run) {
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}
		glClearColor(BackroundScreen[0], BackroundScreen[1], BackroundScreen[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapInterval(vsync);
		camera.updateMatrix(fov, 0.1f, 100.0f);
		glEnable(GL_DEPTH_TEST);


		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ObjectCreator(window, PresceneObjects[selectedObject]);
		
		static bool show_selected_pop = false;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			if (!show_selected_pop) 
			{
				ImVec2 mouse_pos = ImVec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
				ImGui::SetNextWindowPos(mouse_pos, ImGuiCond_Appearing, ImVec2(0, 1));
				ImGui::OpenPopup("Selected Object Settings");
			}
			show_selected_pop = true;
		}
		onpopupmenu = false;
		if (ImGui::BeginPopup("Selected Object Settings"))
		{
			onpopupmenu = true;

			if (ImGui::CollapsingHeader(sceneObjects[selectedObject].name == "" ? std::to_string(selectedObject).c_str() : sceneObjects[selectedObject].name.c_str()))
			{
				if (ImGui::Button(("Delete Object##" + std::to_string(selectedObject)).c_str()))
				{
					DeleteObject action(PresceneObjects[selectedObject]);
					undoStack.push(action);

					PresceneObjects[selectedObject].deleted = true;
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_NAME");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_POS_X");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_POS_Y");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_SCA_X");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_SCA_Y");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_ANGLE");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_TEXTURE");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_LAYER");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_RUNDRAW");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_FRIC");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_STATIC");
					SavingSystem.remove("OBJ" + std::to_string(selectedObject) + "_TRIG");
					PresceneObjects.erase(PresceneObjects.begin() + selectedObject);
					sceneObjects.erase(sceneObjects.begin() + selectedObject);
				}
				char F[128] = "";
				char objName[128];
				strcpy_s(objName, sizeof(objName), PresceneObjects[selectedObject].name.c_str());
				ImGui::InputText(("Obj Name##" + std::to_string(selectedObject)).c_str(), objName, ImGuiInputTextFlags_EnterReturnsTrue);
				if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && strlen(objName) > 0) {
					PresceneObjects[selectedObject].name = objName;
					sceneObjects[selectedObject].name = objName;
					// Create a string to undo
				}
				InputBoolWithEndFocus("Draw ##", &PresceneObjects[selectedObject].drawOnRuntime);

				InputFloatWithEndFocus("Pos X##", &PresceneObjects[selectedObject].position->x, 0.5f, 1, 0);
				
				InputFloatWithEndFocus("Pos Y##", &PresceneObjects[selectedObject].position->y, 0.5f, 1, 0);

				InputFloatWithEndFocus("Scale X##", &PresceneObjects[selectedObject].scale->x, 0.5f, 1, 0);

				InputFloatWithEndFocus("Scale Y##", &PresceneObjects[selectedObject].scale->y, 0.5f, 1, 0);

				InputFloatWithEndFocus("Angle ##", PresceneObjects[selectedObject].angle, 0.5f, 1, 0);

				InputIntWithEndFocus("Layer ##", &PresceneObjects[selectedObject].layer, 1, 1);

				ImGui::Text("Rigidbody Properties");

				InputIntWithEndFocus("Physical Layer ##", &PresceneObjects[selectedObject].Body->layer);
				
				InputFloatWithEndFocus("Friction ##", &PresceneObjects[selectedObject].Body->friction);

				InputFloatWithEndFocus("Bounciness ##", &PresceneObjects[selectedObject].Body->restitution);

				InputBoolWithEndFocus("Trigger ##", &PresceneObjects[selectedObject].Body->isTrigger);

				InputBoolWithEndFocus("Static ##", &PresceneObjects[selectedObject].Body->isStatic);

			}
			ImGui::EndPopup();
		}


		if (ImGui::IsMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
		{
			show_selected_pop = false;
		}




		if (!run) {
			if (!StartPhase) {
				fov = 45;
				glfwSetWindowTitle(window, "OpenVision *(Universal Editor)");
				for (size_t i = 0; i < PresceneObjects.size(); i++)
				{
					*sceneObjects[i].position = PresceneObjects[i].scenePosition;
					*sceneObjects[i].scale = PresceneObjects[i].sceneScale;
					sceneObjects[i].Body->velocity = vec2(0, 0);
					PresceneObjects[i].Body->velocity = vec2(0, 0);
				}
				//sceneObjects[0].Body->GetCollider()->CalculateAABB();
				for (int i = 0; i < sceneObjects.size(); i++) {
					world.RemoveBody(sceneObjects[i].Body);
				}
				StartPhase = true;
			}
			mouseOverUI = false;
			sceneObjects = PresceneObjects;
			
			if (!InputHandler.GetKey(GLFW_KEY_LEFT_CONTROL) || !InputHandler.GetKey(GLFW_KEY_Z)) {
				releasedUndo = true;
			}

			if (InputHandler.GetKey(GLFW_KEY_LEFT_CONTROL) && InputHandler.GetKey(GLFW_KEY_Z) && releasedUndo) {
				undoAction();
				releasedUndo = false;
			}

			//Manifold manifold;
			//CheckCollision(*OV::SearchObjectByName("obj2", sceneObjects)->Body->GetCollider(), *OV::SearchObjectByName("obj1", sceneObjects)->Body->GetCollider(), manifold);
				
			ImGui::Begin("Execute", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			if (ImGui::Button("Run"))
			{
				if (run == false) {

					run = true;
				}
				else if (run == true)
				{

					run = false;
				}
			}
			if (ImGui::Button("Rebuild"))
			{
				rebuild(window, LocalPy);
			}
			if (ImGui::Button("Build"))
			{
				createFolder("BuildGL");
				filesystem::copy("Assets", "BuildGL/Assets");
				
				createFile("BuildGL/ov.ov");
				filesystem::copy("Vision_engine.exe", "BuildGL/Vision_engine.exe");
				filesystem::copy("OpenVisionIcon.png", "BuildGL/OpenVisionIcon.png");
				filesystem::copy("SCENE.ov", "BuildGL/SCENE.ov");
				filesystem::copy("SETTINGS.ov", "BuildGL/SETTINGS.ov");
				filesystem::copy("SCRIPTS.ov", "BuildGL/SCRIPTS.ov");

				

			}
			ImGui::InputInt("PyIndex", &PythonIndex);
			ImGui::Checkbox("Local-Python (not recommended)", &LocalPy);

			if (ImGui::Button("Exit OV"))
			{
				std::system("taskkill /f /im python.exe");
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				std::string processName = "MSBuild.exe";
				std::string processPath = "Build\MSBuild\Current\Bin\"";
				std::string command = "taskkill /f /im " + processName;

				std::cout << "Terminating " << processName << " process...\n";
				int result = std::system(command.c_str());
				if (result == 0) {
					std::cout << "Process terminated successfully.\n";
				}
				else {
					std::cout << "Error: could not terminate process.\n";
				}


			}
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			ImGui::End();

			con.Draw(no_resize,no_move);


			ImGui::Begin("Sources", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			if (ImGui::BeginTabBar("TabBar"))
			{
				if (ImGui::BeginTabItem("Textures"))
				{
					if (ImGui::Button("Add Texture"))
					{
						textures.push_back(Texture(addedfile));
					}

					ImGui::InputText("Path : ", addedfile, sizeof(addedfile));

					for (size_t k = 0; k < textures.size(); k++)
					{
						ImGui::Separator();

						if (ImGui::Selectable(("Bind : " + std::string(textures[k].ImageFile)).c_str()))
						{
							PresceneObjects[selectedObject].tex = textures[k];
							PresceneObjects[selectedObject].texChar = textures[k].FullImageFile;
						}

						ImGui::Separator();
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Graphics"))
				{
					/*
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::Text("MSAA does not work with the current framebuffer.");
					ImGui::PopStyleColor();
					*/
					ImGui::Checkbox("Vertical-Synchronization", &vsync);
					ImGui::Checkbox("External-Framebuffer", &DrawFramebuffer);
					if (DrawFramebuffer) {
						if (ImGui::CollapsingHeader("Vignette"))
						{
							ImGui::InputFloat("Radius", &VigRadius);
							ImGui::InputFloat("Softness", &VigSoftness);
						}
						ImGui::Text("Fast Approximate Anti-Aliasing");
						ImGui::InputFloat("FXAA_SPAN_MAX", &FXAA_SPAN_MAX);
						ImGui::InputFloat("FXAA_REDUCE_MIN", &FXAA_REDUCE_MIN);
						ImGui::InputFloat("FXAA_REDUCE_MUL", &FXAA_REDUCE_MUL);
					}
					else {
						ImGui::InputInt("MSAA Samples", &msaa);
					}
					ImGui::Separator();


					ImGui::Separator();
					ImGui::Text("Backround Color");
					ImGui::ColorEdit3("Background Color", BackroundScreen);
					ImGui::Separator();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			ImGui::End();


			static char scriptName[128] = "";

			ImGui::Begin("Scripts", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			ImGui::Columns(2, nullptr, true);
			ImGui::InputText("Script Name", scriptName, IM_ARRAYSIZE(scriptName));

			if (ImGui::Button("Add Script"))
			{
				std::ofstream outputFile("SCRIPTS.ov", std::ios::app);
				outputFile << scriptName << std::endl;
				outputFile.close();
				addOVscript(scriptName);
				memset(scriptName, 0, sizeof(scriptName));
				rebuild(window, LocalPy);
			}

			if (ImGui::Button("Remove Script"))
			{
				if (scriptName != "Script" && 
					scriptName != "Camera" &&
					scriptName != "CircleCollider" &&
					scriptName != "CollisionManager" &&
					scriptName != "Console" &&
					scriptName != "EBO" &&
					scriptName != "IMGUITheme" &&
					scriptName != "Math" &&
					scriptName != "Object" &&
					scriptName != "OVscriptHandaling" &&
					scriptName != "PhysicsBody" &&
					scriptName != "PhysicsWorld" &&
					scriptName != "PolygonCollider" &&
					scriptName != "Settings" &&
					scriptName != "Tex" &&
					scriptName != "VAO" &&
					scriptName != "Presave" &&
					scriptName != "VBO"
					) {
					std::string scriptname_str = scriptName;
					std::ifstream inputFile("SCRIPTS.ov");
					std::ofstream tempFile("temp.txt");
					std::string line;

					while (std::getline(inputFile, line)) {
						if (line.find(scriptname_str) != std::string::npos) {
							continue;
						}
						tempFile << line << std::endl;
					}

					inputFile.close();
					tempFile.close();
					std::remove("SCRIPTS.ov");
					std::rename("temp.txt", "SCRIPTS.ov");

					removeOVscript(scriptName);
					memset(scriptName, 0, sizeof(scriptName));
					rebuild(window, LocalPy);
				}
			}

			if (ImGui::Button("Open Script"))
			{
				std::string command = "start " + std::string(scriptName) + ".cpp";
				system(command.c_str());
				memset(scriptName, 0, sizeof(scriptName));
			}
			ImGui::NextColumn();
			if (ImGui::Button("Script (gloabl ov script)")) {
				std::string command = "start Script.cpp";
				system(command.c_str());
			}
			for (const auto& line : lines) {
				if (ImGui::Button(line.c_str())) {
					std::string command = "start " + std::string(line) + ".cpp";
					system(command.c_str());
				}
			}
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			ImGui::End();

			ImGui::Begin("Window Control", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::Text("Values here will not be saved.");
			ImGui::PopStyleColor();
			ImGui::Checkbox("No Window Moving", &no_move);
			ImGui::Checkbox("No Window Resize", &no_resize);
			ImGui::EndTabItem();
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			ImGui::End();


			ImGui::Begin("Object Inspector", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));

			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}

			if (ImGui::Button("Add object"))
			{

				PresceneObjects.push_back(Object(verts, ind));
				PresceneObjects[PresceneObjects.size() - 1].name = "obj" + to_string(PresceneObjects.size());
				AddObject action("obj" + to_string(PresceneObjects.size()));
				undoStack.push(action);

			}
			{
				for (size_t i = 0; i < PresceneObjects.size(); i++)
				{

					bool foundLastSelected = false;

					for (int i = PresceneObjects.size() - 1; i >= 0; i--) {

						if (PresceneObjects[i].selected) {

							if (!foundLastSelected) {
								PresceneObjects[i].selected = true;
								selectedObject = i;
								foundLastSelected = true;
							}
							else {
								PresceneObjects[i].selected = false;
							}
						}
					}


					if (PresceneObjects[i].deleted == false) {

						if (ImGui::CollapsingHeader(PresceneObjects[i].name == "" ? std::to_string(i).c_str() : PresceneObjects[i].name.c_str()))
						{
							if (ImGui::Button(("Delete Object##" + std::to_string(i)).c_str()))
							{
								DeleteObject action(PresceneObjects[i]);
								undoStack.push(action);

								PresceneObjects[i].deleted = true;
								SavingSystem.remove("OBJ" + std::to_string(i) + "_NAME");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_POS_X");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_POS_Y");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_SCA_X");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_SC A_Y");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_ANGLE");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_TEXTURE");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_LAYER");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_RUNDRAW");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_FRIC");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_STATIC");
								SavingSystem.remove("OBJ" + std::to_string(i) + "_TRIG");
								PresceneObjects.erase(PresceneObjects.begin() + i);
								sceneObjects.erase(sceneObjects.begin() + i);
							}
							char objName[128];
							strcpy_s(objName, sizeof(objName), PresceneObjects[i].name.c_str());
							ImGui::InputText(("Obj Name##" + std::to_string(i)).c_str(), objName, ImGuiInputTextFlags_EnterReturnsTrue);
							if (glfwGetKey(window, GLFW_KEY_ENTER) && strlen(objName) > 0){
								PresceneObjects[i].name = objName;
								sceneObjects[i].name = objName;
							}

							InputBoolWithEndFocus(("Draw ##" + std::to_string(i)).c_str(), &PresceneObjects[i].drawOnRuntime);
							ImGui::Columns(2, nullptr, true);
							InputFloatWithEndFocus(("Pos X##" + std::to_string(i)).c_str(), &PresceneObjects[i].position->x, 0.3f, 1, 0);
							ImGui::NextColumn();
							InputFloatWithEndFocus(("Pos Y##" + std::to_string(i)).c_str(), &PresceneObjects[i].position->y, 0.3f, 1, 0);

							ImGui::Columns(1, nullptr, true);
							ImGui::Columns(2, nullptr, true);
							InputFloatWithEndFocus(("Scale X##" + std::to_string(i)).c_str(), &PresceneObjects[i].scale->x, 0.3f, 1, 0);
							ImGui::NextColumn();

							InputFloatWithEndFocus(("Scale Y##" + std::to_string(i)).c_str(), &PresceneObjects[i].scale->y, 0.3f, 1, 0);

							ImGui::Columns(1, nullptr, true);
							InputFloatWithEndFocus(("Angle ##" + std::to_string(i)).c_str(), PresceneObjects[i].angle, 0.3f, 1, 0);

							InputIntWithEndFocus(("Layer ##" + std::to_string(i)).c_str(), &PresceneObjects[i].layer, 1, 1);
							
							ImGui::Text("Rigidbody Properties");

							InputIntWithEndFocus(("Physical Layer ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->layer);
 							
							InputFloatWithEndFocus(("Friction ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->friction);

							InputFloatWithEndFocus(("Bounciness ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->restitution);

							ImGui::Columns(1, nullptr, true);
							ImGui::Columns(2, nullptr, true);

							InputBoolWithEndFocus(("Trigger ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->isTrigger);

							ImGui::NextColumn();
							InputBoolWithEndFocus(("Static ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->isStatic);
							ImGui::Columns(1, nullptr, true);

							

						}

						ImGui::Separator();

						if (!onpopupmenu) {
							glfwGetCursorPos(window, &mouseX, &mouseY);
						}
						//camera.Position.x = 0.001f;
						ndcMouseX = (float)mouseX / (float)width * 2.0f - 1.0f;
						ndcMouseY = (float)mouseY / (float)height * 2.0f - 1.0f;
						ndcMouseX *= rattio.x * 3.7;
						ndcMouseY *= rattio.y * 3.7;
						int topIndex = -1; 
						
						float maxZIndex = -std::numeric_limits<float>::infinity(); 

						if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
							CMX += (ndcMouseX - beforeMouseXCam);
							CMY += (ndcMouseY - beforeMouseYCam);

						}


						beforeMouseXCam = ndcMouseX;
						beforeMouseYCam = ndcMouseY;

						for (int i = 0; i < PresceneObjects.size(); i++) {
							if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && 
								(PresceneObjects[i].position->x +CMX - abs(PresceneObjects[i].scale->x) / 2) - camera.Position.x < ndcMouseX &&
								(PresceneObjects[i].position->x +CMX + abs(PresceneObjects[i].scale->x) / 2) + camera.Position.x > ndcMouseX &&
								(PresceneObjects[i].position->y +CMY + abs(PresceneObjects[i].scale->y) / 2) - camera.Position.y > ndcMouseY &&
								(PresceneObjects[i].position->y +CMY - abs(PresceneObjects[i].scale->y) / 2) + camera.Position.y < ndcMouseY && !mouseOverUI)
								
							{
								if (PresceneObjects[i].layer > maxZIndex) {
									topIndex = i;
									maxZIndex = PresceneObjects[i].layer;
								}
							}
						}

						if (topIndex != -1 && !mouseOverUI) {
							if (!PresceneObjects[topIndex].selected) {
								beforeMouseX = ndcMouseX;
								beforeMouseY = ndcMouseY;
							}
							else {
								

								float dx = ndcMouseX - beforeMouseX;
								float dy = ndcMouseY - beforeMouseY;

								PresceneObjects[topIndex].position->x += dx;
								PresceneObjects[topIndex].position->y += dy;

								beforeMouseX = ndcMouseX;
								beforeMouseY = ndcMouseY;
							}

							PresceneObjects[topIndex].selected = true;
						}
						else {
							for (int i = 0; i < PresceneObjects.size(); i++) {
								PresceneObjects[i].selected = false;
							}
						}


						PresceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), CMX, CMY);
						glUseProgram(unlitProgram);
						glUniform4f(glGetUniformLocation(unlitProgram, "color"),0.00, 1.0, 0, 1);

						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glLineWidth(3.0f);
						PresceneObjects[selectedObject].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY);
						glUniform4f(glGetUniformLocation(unlitProgram, "color"), 1.0, 0.00, 0.0, 1);
						glLineWidth(1.5f);
						PresceneObjects[selectedObject].DrawTMP(window, unlitProgram, camera, 
							glm::vec2(PresceneObjects[selectedObject].position->x + CMX, PresceneObjects[selectedObject].position->y + CMY),
							glm::vec2(PresceneObjects[selectedObject].scale->x, PresceneObjects[selectedObject].scale->y));
						glLineWidth(0.0f);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


					}
				}
			}

			
		}
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(0, (-36 / 1.5) / 1.5), glm::vec2(114, 0.5));
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(0,( 36 / 1.5) / 1.5), glm::vec2(114, 0.5));

		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2((61.7 / 1.445) / 1.5, 0), glm::vec2(0.5, 64));
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2((-61.7 / 1.445) / 1.5,0), glm::vec2(0.5, 64));

		if (run) {
			if (StartPhase)
			{
				for (size_t i = 0; i < PresceneObjects.size(); i++) {
					sceneObjects[i].scenePosition = *sceneObjects[i].position;
					sceneObjects[i].sceneScale = *sceneObjects[i].scale;
				}
				PresceneObjects = sceneObjects;
				for (int i = 0; i < sceneObjects.size(); i++) {
					world.AddBody(sceneObjects[i].Body);
				}
				con.CLEAR_CONSOLE();
				fov = 22.45;
				script.Start(con, InputHandler, world, sceneObjects, camera);
				AddedScriptscr.Start(con, InputHandler, world, sceneObjects, camera);

				StartPhase = false;
			}
			if (timeDiff >= fixed_timestep) {
				std::string FPS = std::to_string((1.0 / timeDiff) * counter);
				std::string newTitle = "OpenVision - periidev & itaymadeit ~" + FPS + "FPS";
				glfwSetWindowTitle(window, newTitle.c_str());
				prevTime = crntTime;
				counter = 0;



			
				script.Update(con, InputHandler, world, sceneObjects, camera);
				AddedScriptscr.Update(con, InputHandler, world, sceneObjects, camera);


				world.Step(fixed_timestep);
			}

			
			for (size_t i = 0; i < sceneObjects.size(); i++)
			{
				if (!sceneObjects[i].deleted) {
					glLineWidth(0.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glUseProgram(unlitProgram);
					glUniform4f(glGetUniformLocation(unlitProgram, "color"), sceneObjects[i].OutlineColor.x, sceneObjects[i].OutlineColor.y, sceneObjects[i].OutlineColor.z, 1);
					glLineWidth(sceneObjects[i].outlineWidth);
					if (sceneObjects[i].outlineWidth > 0) {
						sceneObjects[i].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY);
					}
					glLineWidth(0.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

					if (sceneObjects[i].drawOnRuntime == true) {
						sceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), CMX, CMY);
					}
				}
			}
			
		}
		if (DrawFramebuffer && run) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glUseProgram(FramebufferProgram);
			glBindVertexArray(rectVAO);
			glDisable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D, framebufferTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		glfwSwapBuffers(window);
		glfwPollEvents();
		InputHandler.Update(window);
		//enable when final build
		if (build) {
			run = true;
		}

	}
	if (!build) {
		SavingSystem.save("OBJ_AMOUNT", (int)sceneObjects.size());
		for (int i = 0; i < sceneObjects.size(); i++) {

			SavingSystem.save("OBJ" + std::to_string(i) + "_NAME", sceneObjects[i].name);
			SavingSystem.save("OBJ" + std::to_string(i) + "_POS_X", sceneObjects[i].position->x);
			SavingSystem.save("OBJ" + std::to_string(i) + "_POS_Y", sceneObjects[i].position->y);
			SavingSystem.save("OBJ" + std::to_string(i) + "_SCA_X", sceneObjects[i].scale->x);
			SavingSystem.save("OBJ" + std::to_string(i) + "_SCA_Y", sceneObjects[i].scale->y);
			SavingSystem.save("OBJ" + std::to_string(i) + "_ANGLE", *sceneObjects[i].angle);
			SavingSystem.save("OBJ" + std::to_string(i) + "_TEXTURE", sceneObjects[i].texChar);
			SavingSystem.save("OBJ" + std::to_string(i) + "_LAYER", sceneObjects[i].layer);
			SavingSystem.save("OBJ" + std::to_string(i) + "_RUNDRAW", sceneObjects[i].drawOnRuntime);
			SavingSystem.save("OBJ" + std::to_string(i) + "_PHYSLAYER", sceneObjects[i].Body->layer);
			SavingSystem.save("OBJ" + std::to_string(i) + "_FRIC", sceneObjects[i].Body->friction);
			SavingSystem.save("OBJ" + std::to_string(i) + "_BOUNCE", sceneObjects[i].Body->restitution);
			SavingSystem.save("OBJ" + std::to_string(i) + "_STATIC", sceneObjects[i].Body->isStatic);
			SavingSystem.save("OBJ" + std::to_string(i) + "_TRIG", sceneObjects[i].Body->isTrigger);
		}
		SavingSystem.save("BG_COLOR", vec3(BackroundScreen[0], BackroundScreen[1], BackroundScreen[2]));
		SavingSystem.saveToFile("SCENE.ov");

		myData.data = { float(vsync), float(msaa), BackroundScreen[0], BackroundScreen[1], BackroundScreen[2], float(LocalPy), 
			float(PythonIndex),float(DrawFramebuffer), VigRadius, VigSoftness, FXAA_SPAN_MAX, FXAA_REDUCE_MIN, FXAA_REDUCE_MUL, CMX, CMY};

		myData.saveData();
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
