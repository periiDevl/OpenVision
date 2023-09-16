#pragma once
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
#include "SaveSystem.h"
#include"OVLIB.h"

std::vector<Object> PresceneObjects;

SaveSystem SavingSystem;



void clearSavingSystem(int I)
{
	SavingSystem.remove("OBJ" + std::to_string(I) + "_NAME");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_POS_X");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_POS_Y");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_SCA_X");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_SCA_Y");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_ANGLE");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_TEXTURE");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_LAYER");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_RUNDRAW");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_FRIC");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_STATIC");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_TRIG");

	SavingSystem.remove("OBJ" + std::to_string(I) + "_TTX");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_TTY");

	SavingSystem.remove("OBJ" + std::to_string(I) + "TINX");
	SavingSystem.remove("OBJ" + std::to_string(I) + "TINY");
	SavingSystem.remove("OBJ" + std::to_string(I) + "TINZ");
}
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
/*
void playSoundThread(const std::string& filePath)
{
	sf::SoundBuffer buffer;

	if (!buffer.loadFromFile(filePath))
	{
		return;
	}

	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();

	while (sound.getStatus() == sf::Sound::Playing)
	{
		sf::sleep(sf::milliseconds(10)); 
	}

}

void playSound(const std::string& filePath)
{
	std::thread soundThread(playSoundThread, filePath);
	soundThread.detach();
}
*/


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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


double scroll_offset = 45.0;

int PythonIndex = 0;



void ObjectUI(GLFWwindow* window, int i)
{
	if (PresceneObjects[i].name != "MainCameraOvSTD") {
		if (ImGui::Button(("Delete Object##" + std::to_string(i)).c_str()))
		{
			DeleteObject action(PresceneObjects[i]);
			undoStack.push(action);

			PresceneObjects[i].deleted = true;
			clearSavingSystem(i);
			PresceneObjects.erase(PresceneObjects.begin() + i);
			sceneObjects.erase(sceneObjects.begin() + i);
		}
		char objName[128];
		strcpy_s(objName, sizeof(objName), PresceneObjects[i].name.c_str());
		ImGui::InputText(("Obj Name##" + std::to_string(i)).c_str(), objName, ImGuiInputTextFlags_EnterReturnsTrue);
		if (glfwGetKey(window, GLFW_KEY_ENTER) && strlen(objName) > 0) {
			bool nameexists = false;
			for (size_t i = 0; i < PresceneObjects.size(); i++)
			{
				if (objName == PresceneObjects[i].name)
				{
					nameexists = true;
				}
			}
			if (!nameexists) {
				PresceneObjects[i].name = objName;
				sceneObjects[i].name = objName;
			}
		}	
	} 

	if (ImGui::TreeNodeEx(("Visual ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		InputBoolWithEndFocus(("Draw (On runtime) ##" + std::to_string(i)).c_str(), &PresceneObjects[i].drawOnRuntime);
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
		ImGui::Columns(2, nullptr, true);
		InputFloatWithEndFocus(("Tex X##" + std::to_string(i)).c_str(), &PresceneObjects[i].TileX, 0.3f, 1, 0);
		ImGui::NextColumn();
		InputFloatWithEndFocus(("Tex Y##" + std::to_string(i)).c_str(), &PresceneObjects[i].TileY, 0.3f, 1, 0);

		ImGui::Columns(1, nullptr, true);

		float tint[3];


		tint[0] = PresceneObjects[i].tint.x;
		tint[1] = PresceneObjects[i].tint.y;
		tint[2] = PresceneObjects[i].tint.z;

		ImGui::ColorEdit3(("Tint ##" + std::to_string(i)).c_str(), tint);

		PresceneObjects[i].tint.x = tint[0];
		PresceneObjects[i].tint.y = tint[1];
		PresceneObjects[i].tint.z = tint[2];

		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx(("Rigidbody Properties ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		InputIntWithEndFocus(("Physical Layer ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->layer);

		InputFloatWithEndFocus(("Friction ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->friction);

		InputFloatWithEndFocus(("Bounciness ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->restitution);

		ImGui::Columns(1, nullptr, true);
		ImGui::Columns(2, nullptr, true);

		InputBoolWithEndFocus(("Trigger ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->isTrigger);

		ImGui::NextColumn();
		InputBoolWithEndFocus(("Static ##" + std::to_string(i)).c_str(), &PresceneObjects[i].Body->isStatic);
		ImGui::Columns(1, nullptr, true);

		ImGui::TreePop();

	}


}

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

	glfwSetWindowShouldClose(window, GLFW_TRUE);
}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll_offset += yoffset;

}



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
					clearSavingSystem(i);

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

namespace fs = std::filesystem;


int main()
{

	std::cout << OVObjects.size();

	
	
	fs::path currentPath = fs::current_path();
	fs::path ProjectName = currentPath.filename().string();

	Presave<float> Data;

	Data = Presave < float >();
	Data.SafeOperation();
	
	bool vsync = Data.data[0];
	int msaa = Data.data[1];
	bool LocalPy = Data.data[5];
	PythonIndex = Data.data[6];
	bool DrawFramebuffer = Data.data[7];
	float VigRadius = Data.data[8];
	float VigSoftness = Data.data[9];
	float FXAA_SPAN_MAX = Data.data[10];
	float FXAA_REDUCE_MIN = Data.data[11];
	float FXAA_REDUCE_MUL = Data.data[12];
	float CMX = Data.data[13];
	float CMY = Data.data[14];
	bool Nearest = Data.data[15];
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
	
	PhysicsWorld world(vec3(0, -55.0f, 0), 10);


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
	Texture nulltex = Texture("");
	//Texture CenterDot = Texture("EngineAssets/CenterDot.png");

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
		float posx, posy, scalex, scaley, angle, layer, restitution, friction, velX, velY, tilex, tiley;
		bool runtimeDraw, isStatic, isTrigger;
		glm::vec4 tint;
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

		tilex = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_TTX", 0.0f);
		tiley = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_TTY", 0.0f);

		tint.x = SavingSystem.getFloat("OBJ" + std::to_string(i) + "TINX", 0.0f);
		tint.y = SavingSystem.getFloat("OBJ" + std::to_string(i) + "TINY", 0.0f);
		tint.z = SavingSystem.getFloat("OBJ" + std::to_string(i) + "TINZ", 0.0f);

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

		obj.TileX = tilex;
		obj.TileY = tiley;
		obj.tint = glm::vec4(tint.x, tint.y, tint.z, 1);

		obj.tex = Texture((texture).c_str());
		PresceneObjects.push_back(obj);
	}

	Object blackbox = Object(verts, ind);
	//Engine Assets
	
	Texture EngineOVObjectIconGui("EngineAssets/ObjectIcon.png");
	Texture EngineOVCameraIconGui("EngineAssets/CameraIcon.png");
	Texture EngineOVTrashIconGui("EngineAssets/OvTrashIcon.png");
	Texture EngineOVScriptIconGui("EngineAssets/OvScriptIcon.png");
	Texture EngineOVRebuildIconGui("EngineAssets/OvRebuildIcon.png");
	Texture EngineOVRunIconGui("EngineAssets/OvRunIcon.png");
	Texture EngineOVBuildIconGui("EngineAssets/OvBuildIcon.png");
	Texture EngineOVMMSIconGui("EngineAssets/mmsicon.png");
	Texture EngineCuserIconGui("EngineAssets/Curser.png");

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	int selectedObject = 0;
	float fov = 45;
	bool onpopupmenu = false;

	bool releasedUndo = true;

	

	const float fixed_timestep = 1.0f / 60.0;
	DefaultTheme();
	glEnable(GL_MULTISAMPLE);

	sceneObjects = PresceneObjects;
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i].scenePosition = *PresceneObjects[i].position;
		sceneObjects[i].sceneScale = *PresceneObjects[i].scale;
		*sceneObjects[i].angle = float(*PresceneObjects[i].angle);
	}
	
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

	bool firsttime = true;
	ImGuiStyle& style = ImGui::GetStyle();
	float originalButtonPadding = style.FramePadding.y;
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

		if (DrawFramebuffer) {
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
		if (PresceneObjects.size() > 1) {
			ObjectCreator(window, PresceneObjects[selectedObject]);
		}
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
				ObjectUI(window, selectedObject);
			}
			ImGui::EndPopup();
		}


		if (ImGui::IsMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
		{
			show_selected_pop = false;
		}


#pragma region Pre-Runtime

		if (!run) {
			if (!StartPhase) {
				if (!firsttime) {
				script.Exit();
				}
				firsttime = false;

				camera.Position.x = 0;
				camera.Position.y = 0;

				sf::Listener::setGlobalVolume(0);
				fov = 45;
				std::string title = "OpenVision *(Universal Editor) ~ *" + ProjectName.string();
				glfwSetWindowTitle(window, title.c_str());
				for (size_t i = 0; i < PresceneObjects.size(); i++)
				{
					*sceneObjects[i].position = PresceneObjects[i].scenePosition;
					*sceneObjects[i].scale = PresceneObjects[i].sceneScale;
					sceneObjects[i].Body->SetVelocity(vec2(0, 0));
					sceneObjects[i].angle = new float;
					*sceneObjects[i].angle = *PresceneObjects[i].angle;
					PresceneObjects[i].Body->SetVelocity(vec2(0, 0));

					if (PresceneObjects[i].name == "MainCameraOvSTD") {
						PresceneObjects[i].tex = EngineOVCameraIconGui;
					}

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

#pragma endregion Pre-Runtime

#pragma region UI
			ImTextureID RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVTrashIconGui.ID));
			//Manifold manifold;
			//CheckCollision(*OV::SearchObjectByName("obj2", sceneObjects)->Body->GetCollider(), *OV::SearchObjectByName("obj1", sceneObjects)->Body->GetCollider(), manifold);
			ImGui::BeginMainMenuBar();
			


			ImGui::Separator();

			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("Save"))
				{
					//save
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Debug"))
			{
				RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVMMSIconGui.ID));
				if (ImGui::ImageButton(RebuildimguiTextureID, ImVec2(30, 30)))
				{
				}
				ImGui::SameLine(); 
				ImGui::Text("Mono-MS");

				ImGui::Separator();

				if (ImGui::ImageButton(RebuildimguiTextureID, ImVec2(30, 30)))
				{
				}
				ImGui::SameLine();
				ImGui::Text("CUGNYSIS");



				ImGui::EndMenu();
			}

			ImGui::Separator();


			ImGui::EndMainMenuBar();
			
			ImGui::End();
			ImGui::Begin("Execute", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			ImGuiStyle& style = ImGui::GetStyle();


			ImVec2 originalButtonTextAlign = style.ButtonTextAlign;
			ImVec2 originalFramePadding = style.FramePadding;

			
			style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
			style.FramePadding = ImVec2(1.0f, 1.0f);


			style.ButtonTextAlign = originalButtonTextAlign;
			style.FramePadding = originalFramePadding;


			RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVRunIconGui.ID));
			if (ImGui::ImageButton(RebuildimguiTextureID, ImVec2(30, 30)))
			{
				if (run == false) {

					run = true;
				}
				else if (run == true)
				{

					run = false;
				}

			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text("Run");

				ImGui::EndTooltip();
			}
			ImGui::SameLine();
			RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVRebuildIconGui.ID));

			if (ImGui::ImageButton(RebuildimguiTextureID, ImVec2(30, 30)))
			{
				rebuild(window, LocalPy);
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text("Re-build project");

				ImGui::EndTooltip();
			}

			ImGui::SameLine();
			RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVBuildIconGui.ID));

			if (ImGui::ImageButton(RebuildimguiTextureID, ImVec2(30, 30)))
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
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text("Export project");

				ImGui::EndTooltip();
			}

			ImGui::SameLine();
			RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVTrashIconGui.ID));

			if (ImGui::ImageButton(RebuildimguiTextureID, ImVec2(30, 30)))
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
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text("Exit program");

				ImGui::EndTooltip();
			}

			style.ButtonTextAlign = originalButtonTextAlign;
			style.FramePadding = originalFramePadding;

			//move later
			//ImGui::InputInt("PyIndex", &PythonIndex);
			//ImGui::Checkbox("Local-Python (not recommended)", &LocalPy);


			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			ImGui::End();

			con.Draw(no_resize,no_move);
			if (con.isConsoleHove)
			{
				mouseOverUI = true;
			}

			ImGui::End();
			ImGui::Begin("Sources", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			if (ImGui::BeginTabBar("TabBar"))
			{
				if (ImGui::BeginTabItem("Object Settings"))
				{
					ObjectUI(window,selectedObject);

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Textures"))
				{
					if (ImGui::Button("Refresh"))
					{
						textures.clear();
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
					}


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
					ImGui::Separator();
					if (DrawFramebuffer) {
						if (ImGui::TreeNode("Framebuffer-Effects"))
						{
							if (ImGui::TreeNode("Vignette")) {
								ImGui::InputFloat("Radius", &VigRadius);
								ImGui::InputFloat("Softness", &VigSoftness);
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Anti-Aliasing")) {
								
								ImGui::InputFloat("FXAA_SPAN_MAX", &FXAA_SPAN_MAX);
								ImGui::InputFloat("FXAA_REDUCE_MIN", &FXAA_REDUCE_MIN);
								ImGui::InputFloat("FXAA_REDUCE_MUL", &FXAA_REDUCE_MUL);
								ImGui::TreePop();
							}
							ImGui::TreePop();
						}
					}
					else {
						if (ImGui::TreeNode("Anti-Aliasing")) {

							ImGui::InputInt("MSAA Samples", &msaa);
							ImGui::TreePop();
						}
					}
					ImGui::Separator();

					if (ImGui::TreeNode("Texture-Settings"))
					{
						static const char* items[] = { "GL_NEAREST", "GL_LINEAR"};
						static int currentItem = 0;
						ImGui::Text("Texture filtering :");
						ImGui::Combo("##combo", &currentItem, items, IM_ARRAYSIZE(items));

						if (currentItem == 0) {
							Nearest = true;
						}
						else if (currentItem == 1) {
							Nearest = false;
						}

						
						ImGui::TreePop();
					}

					ImGui::Separator();
					ImGui::Text("Backround Color");
					ImGui::ColorEdit3("Background Color", BackroundScreen);
					ImGui::Separator();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::End();


			static char scriptName[128] = "";

			ImGui::End();
			ImGui::Begin("Scripts", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			
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
			




			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			ImGui::End();

			ImGui::End();
			ImGui::Begin("Scripts Select", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			if (ImGui::Button("Script (gloabl ov script)")) {
				std::string command = "start Script.cpp";
				system(command.c_str());
			}
			ImTextureID imguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVScriptIconGui.ID));
			int itemsPerRow = 3;
			int rowCount = (lines.size() + itemsPerRow - 1) / itemsPerRow;


			
			float gridWidth = itemsPerRow * (500 + ImGui::GetStyle().ItemSpacing.x);

			
			ImGui::BeginChild("GridChild", ImVec2(gridWidth, 0), false);
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			for (int row = 0; row < rowCount; ++row) {
				for (int col = 0; col < itemsPerRow; ++col) {
					int index = row * itemsPerRow + col;

					if (index < lines.size()) {
						const auto& line = lines[index];

						ImGui::PushID(index);
						ImGui::BeginGroup();

						if (ImGui::ImageButton((ImTextureID)imguiTextureID, ImVec2(50, 50))) {
							std::string command = "start " + line + ".cpp";
							system(command.c_str());
						}
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							
							ImGui::Text(("Edit \"" + line + "\" Script.").c_str());
							
							ImGui::EndTooltip();
						}

						ImVec2 textPosition = ImGui::GetItemRectMin();
						textPosition.y += 50;
						textPosition.x += (50 - ImGui::CalcTextSize(line.c_str()).x) * 0.5f + 10;
						ImGui::SetCursorScreenPos(textPosition);
						ImGui::Text("%s", line.c_str());

						ImGui::EndGroup();
						ImGui::PopID();
					}

					ImGui::SameLine();
				}
				ImGui::NewLine();
			}
			ImGui::EndChild();



			ImGui::End();
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
						if (i == selectedObject) {
							ImGui::SetNextItemOpen(true);
						}

						ImGui::Separator();

						ImTextureID imguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVObjectIconGui.ID));
						if (PresceneObjects[i].name != "MainCameraOvSTD") {
							imguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVObjectIconGui.ID));
						}
						else if (PresceneObjects[i].name == "MainCameraOvSTD") {
							imguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVCameraIconGui.ID));
						}


						
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						style.FramePadding = ImVec2(1.0f, 1.0f);

						ImVec2 imageSize(28, 28);
						if (ImGui::ImageButton(imguiTextureID, imageSize))
						{
						}

						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							if (PresceneObjects[i].name != "MainCameraOvSTD") {
								ImGui::Text("An OpenVision regular object.");
							}
							else if (PresceneObjects[i].name == "MainCameraOvSTD") {
								ImGui::Text("OpenVision's camera object (displayes the object's to the screen)");
							}
							ImGui::EndTooltip();
						}

						style.ButtonTextAlign = originalButtonTextAlign;
						style.FramePadding = originalFramePadding;

						ImGui::SameLine();

						
						

						style.FramePadding.y = 8.0f;

						if (ImGui::CollapsingHeader(PresceneObjects[i].name.c_str()))
						{
							style.FramePadding.y = originalButtonPadding;
							ObjectUI(window, i);
						}
						style.FramePadding.y = originalButtonPadding;



						ImGui::Separator();

						if (!onpopupmenu) {
							glfwGetCursorPos(window, &mouseX, &mouseY);
							
						}
#pragma endregion UI


#pragma region Mouse-Detection
						/*
						if (!mouseOverUI)
						{
							glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

						}
						else {
							glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

						}
						*/
						//camera.Position.x = 0.001f;
						ndcMouseX = (float)mouseX / (float)width * 2.0f - 1.0f;
						ndcMouseY = -((float)mouseY / (float)height * 2.0f - 1.0f);
						ndcMouseX *= rattio.x * 3.65;
						ndcMouseY *= rattio.y * 3.65;
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


						glUseProgram(unlitProgram);
						glUniform4f(glGetUniformLocation(unlitProgram, "color"),0.00, 1.0, 0, 1);

						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glLineWidth(3.0f);
						PresceneObjects[selectedObject].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
						glUniform4f(glGetUniformLocation(unlitProgram, "color"), 1.0, 0.00, 0.0, 1);
						glLineWidth(1.5f);
						PresceneObjects[selectedObject].DrawTMP(window, unlitProgram, camera, 
							glm::vec2(PresceneObjects[selectedObject].position->x + CMX, PresceneObjects[selectedObject].position->y + CMY),
							glm::vec2(PresceneObjects[selectedObject].scale->x, PresceneObjects[selectedObject].scale->y));
						glLineWidth(0.0f);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

						PresceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);

					}
				}
			}

			
		}
#pragma endregion Mouse-Detection

		Object RuntimeCam = *OV::SearchObjectByName("MainCameraOvSTD", PresceneObjects);
		
		blackbox.tex = nulltex;
		blackbox.layer = 100;
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(RuntimeCam.position->x, ((-36 / 1.5) / 1.5) + RuntimeCam.position->y), glm::vec2(59, 0.5));
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(RuntimeCam.position->x, ((36 / 1.5) / 1.5) + RuntimeCam.position->y), glm::vec2(59, 0.5));

		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2((61.7 / 1.445 / 1.5) + RuntimeCam.position->x, RuntimeCam.position->y), glm::vec2(0.5, 33.8));
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2((-61.7 / 1.445 / 1.5) + RuntimeCam.position->x, RuntimeCam.position->y), glm::vec2(0.5, 33.8));

		blackbox.tex = EngineCuserIconGui;
		/*
		if (!mouseOverUI) {
			blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(ndcMouseX, ndcMouseY), glm::vec2(2.5, 2.5));
		}
		*/


		//blackbox.tex = CenterDot;
		//blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(CMX,CMY), glm::vec2(5, 5));
#pragma region Runtime
		if (run) {
			
			if (StartPhase)
			{
				
				OVObjects.clear();
				sf::Listener::setGlobalVolume(100);
				for (size_t i = 0; i < PresceneObjects.size(); i++) {
					PresceneObjects[i].scenePosition = *sceneObjects[i].position;
					PresceneObjects[i].sceneScale = *sceneObjects[i].scale;
					PresceneObjects[i].angle = new float;
					*PresceneObjects[i].angle = *sceneObjects[i].angle;

					OVObjects.push_back(Ov_Object{ PresceneObjects[i].scenePosition.x, PresceneObjects[i].scenePosition.y, PresceneObjects[i].sceneScale.x, PresceneObjects[i].sceneScale.y});
				}
				
				for (int i = 0; i < sceneObjects.size(); i++) {
					world.AddBody(sceneObjects[i].Body);
				}
				//std::cout << OVObjects.size();
				/*
				for (size_t i = 0; i < OVObjects.size(); i++)
				{
					std::cout << OVObjects[i].x;
				}
				*/
				//OVObjects = sceneObjects;

				con.CLEAR_CONSOLE();
				fov = 22.45;
				script.Start();
				//sharedVar = 15;

				int sharedVar = GetSharedVar();
				sharedVar = 89;
				SetSharedVar(sharedVar);
				ScriptStart();
				//std::cout << sharedVar;
				StartPhase = false;
			}

				script.Update();

				ScriptUpdate();

				for (size_t i = 0; i < sceneObjects.size(); i++)
				{
					if (sceneObjects[i].Body->getVelocity() != vec2(0.0f) && sceneObjects[i].Body->isStatic) {
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
						cout << "invalid object name:" << sceneObjects[i].name << endl;
					}
				}
				
			if (glfwWindowShouldClose(window))
			{
				script.Exit();
			}
			if (timeDiff >= fixed_timestep) {
				std::string FPS = std::to_string((1.0 / timeDiff) * counter);
				std::string newTitle = ProjectName.string() + " ~" + FPS + "FPS";
				glfwSetWindowTitle(window, newTitle.c_str());
				prevTime = crntTime;
				counter = 0;



				camera.Position.x = RuntimeCam.position->x;
				camera.Position.y = -RuntimeCam.position->y;


				world.Step(fixed_timestep);
			}
			//OVObjects[1].scale_x = 100;
			
			for (size_t i = 0; i < sceneObjects.size(); i++)
			{
				
				sceneObjects[i].scenePosition = glm::vec2(OVObjects[i].x, OVObjects[i].y);
				sceneObjects[i].sceneScale = glm::vec2(OVObjects[i].scale_x, OVObjects[i].scale_y);
				
				
				if (!sceneObjects[i].deleted) {
					glLineWidth(0.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glUseProgram(unlitProgram);
					glUniform4f(glGetUniformLocation(unlitProgram, "color"), sceneObjects[i].OutlineColor.x, sceneObjects[i].OutlineColor.y, sceneObjects[i].OutlineColor.z, 1);
					glLineWidth(sceneObjects[i].outlineWidth);
					if (sceneObjects[i].outlineWidth > 0) {
						sceneObjects[i].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY,Nearest);
					}
					glLineWidth(0.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					if (sceneObjects[i].drawOnRuntime == true) {
						sceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
					}
				}
			}
			
		}
		if (DrawFramebuffer) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glUseProgram(FramebufferProgram);
			glBindVertexArray(rectVAO);
			glDisable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D, framebufferTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		

#pragma endregion Runtime


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
			SavingSystem.save("OBJ" + std::to_string(i) + "_ANGLE", *PresceneObjects[i].angle);
			SavingSystem.save("OBJ" + std::to_string(i) + "_TEXTURE", sceneObjects[i].texChar);
			SavingSystem.save("OBJ" + std::to_string(i) + "_LAYER", sceneObjects[i].layer);
			SavingSystem.save("OBJ" + std::to_string(i) + "_RUNDRAW", sceneObjects[i].drawOnRuntime);
			SavingSystem.save("OBJ" + std::to_string(i) + "_PHYSLAYER", sceneObjects[i].Body->layer);
			SavingSystem.save("OBJ" + std::to_string(i) + "_FRIC", sceneObjects[i].Body->friction);
			SavingSystem.save("OBJ" + std::to_string(i) + "_BOUNCE", sceneObjects[i].Body->restitution);
			SavingSystem.save("OBJ" + std::to_string(i) + "_STATIC", sceneObjects[i].Body->isStatic);
			SavingSystem.save("OBJ" + std::to_string(i) + "_TRIG", sceneObjects[i].Body->isTrigger);

			SavingSystem.save("OBJ" + std::to_string(i) + "_TTX", sceneObjects[i].TileX);
			SavingSystem.save("OBJ" + std::to_string(i) + "_TTY", sceneObjects[i].TileY);

			SavingSystem.save("OBJ" + std::to_string(i) + "TINX", sceneObjects[i].tint.x);
			SavingSystem.save("OBJ" + std::to_string(i) + "TINY", sceneObjects[i].tint.y);
			SavingSystem.save("OBJ" + std::to_string(i) + "TINZ", sceneObjects[i].tint.z);
		}
		SavingSystem.save("BG_COLOR", vec3(BackroundScreen[0], BackroundScreen[1], BackroundScreen[2]));
		SavingSystem.saveToFile("SCENE.ov");

		Data.data = { float(vsync), float(msaa), BackroundScreen[0], BackroundScreen[1], BackroundScreen[2], float(LocalPy), 
			float(PythonIndex),float(DrawFramebuffer), VigRadius, VigSoftness, FXAA_SPAN_MAX, FXAA_REDUCE_MIN, FXAA_REDUCE_MUL, CMX, CMY, float(Nearest)};

		Data.saveData();
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
