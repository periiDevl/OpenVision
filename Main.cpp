#pragma once
#include"OV.h"
#include "Settings.h"
#include "FileFunctions.h"
#include "SteamOVIMP.h"
#include "Object.h"
#include "Math.h"
#include "IMGUITheme.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stack>
#include <variant>
#include <filesystem>
#include "PhysicsWorld.h"
#include <thread>
#include "Console.h"
#include "OVscriptHandaling.h"
#include "Presave.h"
#include "Script.h"
#include <chrono>
#include "SaveSystem.h"
#include "OVLIB.h"
#include "Settings.h"
#include "MenuItems.h"
SaveSystem SavingSystem;
//find an apropriate space
//----------------
bool ScaleXBool = true;
bool stopWarning = false;
int animationKey = 0;
//----------------
 

//Move to seprate calss---
void popupforinstall() {
	if (!stopWarning) {
		ImGui::OpenPopup("Warning");
	}


	if (ImGui::BeginPopupModal("Warning")) {

		ImGui::Text("The pakage manager of mods, is essentially a free wrokshop for anyone to use ");
		ImGui::Text("and prone to viruses and malicious software if not used properly.");
		ImGui::Text("We recommend to use this in moderation or by only using mods approved by our team. (Offical mods)");
		if (ImGui::Button("Acknowledge")) {
			stopWarning = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
//----
void SaveObjectsToFile(const std::string& filename) {
	std::ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		std::cerr << "Error: Unable to open the output file." << std::endl;
		return;
	}

	for (const Object& object : sceneObjects) {
		glm::vec2* pos = object.position;
		float posx = pos->x;
		float posy = pos->y;
		glm::vec2* scale = object.scale;
		float scalex = scale->x;
		float scaley = scale->y;
		outputFile << posx << " " << posy << " " << scalex << " " << scaley << std::endl;
	}

	outputFile.close();
}

std::vector<float> splitStringToFloats(const std::string& input) {
	std::vector<float> floatArray;
	std::stringstream ss(input);
	std::string token;

	while (std::getline(ss, token, ',')) {
		float value = std::stof(token);
		floatArray.push_back(value);
	}

	return floatArray;
}

glm::vec3 lerp(const glm::vec3& start, const glm::vec3& end, float t) {
	return start + t * (end - start);
}

glm::vec3 interpolateKeyframes(const glm::vec3& start, const glm::vec3& end, int startKeyframe, int endKeyframe, int currentKeyframe) {
	if (currentKeyframe < startKeyframe) return start;
	if (currentKeyframe > endKeyframe) return end;

	float t = (currentKeyframe - startKeyframe) / static_cast<float>(endKeyframe - startKeyframe);

	return lerp(start, end, t);
}

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
	SavingSystem.remove("OBJ" + std::to_string(I) + "_DFRIC");
	SavingSystem.remove("OBJ" + std::to_string(I) + "_SFRIC");
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

	ModifyGuiInt(const int v, int* memory) : wasValue(v), valueInMemory(memory) {}

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

//Move to serpate calss 
//----

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
bool InputFloatSliderWithEndFocus(const char* label, float* value, float min = 0.0f, float max = 0.0f, ImGuiInputTextFlags flags = 0)
{
	std::ostringstream formatStream;
	formatStream << "%" << min << "f";
	std::string formatString = formatStream.str();

	bool valueChanged = ImGui::SliderFloat(label, value, min, max, formatString.c_str(), flags);
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
//-----


void ObjectCreator(GLFWwindow* window, Object obj) {
	static bool hasExecuted = false;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) && glfwGetKey(window, GLFW_KEY_D) && !hasExecuted) {
		sceneObjects.push_back(Object(verts, ind));
		sceneObjects[sceneObjects.size() - 1].name = "obj Copyof (" + obj.name + ") id (" + to_string(sceneObjects.size()) + ")";
		//sceneObjects[sceneObjects.size() - 1].position = obj.position;
		sceneObjects[sceneObjects.size() - 1].tex = obj.tex;
		sceneObjects[sceneObjects.size() - 1].texChar = obj.texChar;
		sceneObjects[sceneObjects.size() - 1].scale = obj.scale;
		sceneObjects[sceneObjects.size() - 1].angle = obj.angle;
		hasExecuted = true;
	}

	if (!glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || !glfwGetKey(window, GLFW_KEY_D)) {
		hasExecuted = false;
	}
}


Script script;

//(delete later)
bool MouseOverObject(Object obj, Camera& camera, glm::vec3 axis, float cmx, float cmy, float mx, float my) {
	// Using SAT to check if the mouse is over the object "a"
	vector<vec2> vertices = obj.GetVertices(camera, axis, cmx, cmy);

	return CollisionManager::PolyVPoint(vertices, vec2(mx, my));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Find a more apropriate sapce
//-----------------
double scroll_offset = 45.0f;
int BindSelectedObject = -1;
bool scaling = false;
float initialMouseX = 0.0f;
float initialScaleX = 1.0f;
//-------------------

void ObjectUI(GLFWwindow* window, int i, Texture EngineLayerIconGui)
{
	ImTextureID Texture = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineLayerIconGui.ID));
	
	if (sceneObjects[i].parent != -1)
	{
		sceneObjects[i].parent_position = sceneObjects[sceneObjects[i].parent].position;
		if (ImGui::Button(("Remove from parent##" + std::to_string(i)).c_str()))
		{
			sceneObjects[i].parent = -1;
		}
	}

	if (sceneObjects[i].name != "MainCameraOvSTD") {
		if (ImGui::Button(("Delete Object##" + std::to_string(i)).c_str()))
		{
			DeleteObject action(sceneObjects[i]);
			undoStack.push(action);

			sceneObjects[i].deleted = true;
			clearSavingSystem(i);
			sceneObjects.erase(sceneObjects.begin() + i);
		}
		char objName[128];
		strcpy_s(objName, sizeof(objName), sceneObjects[i].name.c_str());
		ImGui::InputText(("Obj Name##" + std::to_string(i)).c_str(), objName, ImGuiInputTextFlags_EnterReturnsTrue);
		if (glfwGetKey(window, GLFW_KEY_ENTER) && strlen(objName) > 0) {
			bool nameexists = false;
			for (size_t i = 0; i < sceneObjects.size(); i++)
			{
				if (objName == sceneObjects[i].name)
				{
					nameexists = true;
				}
			}
			if (!nameexists) {
				sceneObjects[i].name = objName;
			}
		}

		
	}

	if (ImGui::TreeNodeEx(("Visual ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		InputBoolWithEndFocus(("Draw (On runtime) ##" + std::to_string(i)).c_str(), &sceneObjects[i].drawOnRuntime);
		ImGui::Columns(2, nullptr, true);
		InputFloatWithEndFocus(("Pos X##" + std::to_string(i)).c_str(), &sceneObjects[i].position->x, 0.3f, 1, 0);
		ImGui::NextColumn();
		InputFloatWithEndFocus(("Pos Y##" + std::to_string(i)).c_str(), &sceneObjects[i].position->y, 0.3f, 1, 0);

		ImGui::Columns(1, nullptr, true);
		ImGui::Columns(2, nullptr, true);
		InputFloatWithEndFocus(("Scale X##" + std::to_string(i)).c_str(), &sceneObjects[i].scale->x, 0.3f, 1, 0);
		ImGui::NextColumn();

		InputFloatWithEndFocus(("Scale Y##" + std::to_string(i)).c_str(), &sceneObjects[i].scale->y, 0.3f, 1, 0);

		ImGui::Columns(1, nullptr, true);
		InputFloatWithEndFocus(("Angle ##" + std::to_string(i)).c_str(), sceneObjects[i].angle, 0.3f, 1, 0);

		if (ImGui::ImageButton(Texture, ImVec2(20, 20)))
		{
		}
		ImGui::SameLine();
		InputIntWithEndFocus(("Layer ##" + std::to_string(i)).c_str(), &sceneObjects[i].layer, 1, 1);

		ImGui::Separator();

		ImGui::Columns(2, nullptr, true);
		InputFloatWithEndFocus(("Tex X##" + std::to_string(i)).c_str(), &sceneObjects[i].TileX, 0.3f, 1, 0);
		ImGui::NextColumn();
		InputFloatWithEndFocus(("Tex Y##" + std::to_string(i)).c_str(), &sceneObjects[i].TileY, 0.3f, 1, 0);

		ImGui::Columns(1, nullptr, true);

		float tint[3];


		tint[0] = sceneObjects[i].tint.x;
		tint[1] = sceneObjects[i].tint.y;
		tint[2] = sceneObjects[i].tint.z;

		ImGui::ColorEdit3(("Tint ##" + std::to_string(i)).c_str(), tint);

		sceneObjects[i].tint.x = tint[0];
		sceneObjects[i].tint.y = tint[1];
		sceneObjects[i].tint.z = tint[2];

		ImGui::TreePop();
	}

	// Im lazy to fix it now, will be fixed later now I want to check if this is even working 
	if (ImGui::TreeNodeEx(("Rigidbody Properties ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		InputIntWithEndFocus(("Physical Layer ##" + std::to_string(i)).c_str(), &sceneObjects[i].Body->layer);

		InputFloatWithEndFocus(("Dynamic Friction ##" + std::to_string(i)).c_str(), &sceneObjects[i].Body->dynamicFric);

		InputFloatWithEndFocus(("Static Friction ##" + std::to_string(i)).c_str(), &sceneObjects[i].Body->staticFric);

		InputFloatWithEndFocus(("Density ##" + std::to_string(i)).c_str(), &sceneObjects[i].Body->density);
	
		InputFloatWithEndFocus(("Bounciness ##" + std::to_string(i)).c_str(), &sceneObjects[i].Body->restitution);
	
		ImGui::Columns(1, nullptr, true);
		ImGui::Columns(2, nullptr, true);
	
		bool triggerValue = sceneObjects[i].Body->IsTrigger();
		InputBoolWithEndFocus(("Trigger ##" + std::to_string(i)).c_str(), &triggerValue);
		sceneObjects[i].Body->IsTrigger(triggerValue);

		ImGui::NextColumn();
		bool staticValue = sceneObjects[i].Body->IsStatic();
		InputBoolWithEndFocus(("Static ##" + std::to_string(i)).c_str(), &staticValue);
		sceneObjects[i].Body->IsStatic(staticValue);
		ImGui::Columns(1, nullptr, true);
	
		ImGui::TreePop();
	
	}

	if (ImGui::TreeNodeEx(("Rigidbody Properties ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

		InputFloatSliderWithEndFocus(("cornerRadius ##" + std::to_string(i)).c_str(), &sceneObjects[i].cornerRadius, 0.01, 0.5);


		ImGui::TreePop();

	}


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


void undoAction() {
	if (!undoStack.empty()) {
		Action previousAction = undoStack.top();
		undoStack.pop();

		if (const AddObject* addObjectAction = std::get_if<AddObject>(&previousAction)) {

			for (size_t i = 0; i < sceneObjects.size(); i++)
			{
				if (sceneObjects[i].name == addObjectAction->name) {

					sceneObjects[i].deleted = true;
					clearSavingSystem(i);

					sceneObjects.erase(sceneObjects.begin() + i);
					sceneObjects.erase(sceneObjects.begin() + i);
					return;
				}
			}
		}
		else if (const DeleteObject* deleteObjectAction = std::get_if<DeleteObject>(&previousAction)) {

			sceneObjects.push_back(deleteObjectAction->object);
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




int main2()
{


	//Determind if the program is in runtime or editor.
	bool build = false;
	
	//Use Steam api?
	Steam(false, false);

	//Find the path and choose a name for the project by the folder.
	fs::path currentPath = fs::current_path();
	fs::path ProjectName = currentPath.filename().string();

	//Get the data from the save file
	//Improve system
	//----------------------
	Presave<float> Data;

	Data = Presave < float >();
	Data.SafeOperation();

	bool vsync = Data.data[0];
	int msaa = Data.data[1];
	bool DrawFramebuffer = Data.data[5];
	float VigRadius = Data.data[6];
	float VigSoftness = Data.data[7];
	float FXAA_SPAN_MAX = Data.data[8];
	float FXAA_REDUCE_MIN = Data.data[9];
	float FXAA_REDUCE_MUL = Data.data[10];
	float CMX = Data.data[11];
	float CMY = Data.data[12];

	bool Nearest = Data.data[13];
	//------------------------

	//Get the list of scripts included in the editor.
	//find a better way to do this
	//--------------------------
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
	//--------------------------

	//Initilize glfw
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
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	DefaultTheme();
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 originalItemSpacing = ImGui::GetStyle().ItemSpacing;
	float originalButtonPadding = style.FramePadding.y;

	//Create the shaders for OpenVision.
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

	GLuint fragmentShaderMouseDetecting = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderMouseDetecting, 1, &ObjectIDUnlitFragment, NULL);
	glCompileShader(fragmentShaderMouseDetecting);

	GLuint MouseDetectionProgram = glCreateProgram();
	glAttachShader(MouseDetectionProgram, vertexShader);
	glAttachShader(MouseDetectionProgram, fragmentShaderMouseDetecting);
	glLinkProgram(MouseDetectionProgram);

	glUseProgram(FramebufferProgram);
	glUniform1i(glGetUniformLocation(FramebufferProgram, "screenTexture"), 0);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "radius"), VigRadius);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "softness"), VigSoftness);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "minEdgeContrast"), FXAA_REDUCE_MIN);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "subPixelAliasing"), FXAA_REDUCE_MUL);
	glUniform1f(glGetUniformLocation(FramebufferProgram, "maximumEdgeDetection"), FXAA_SPAN_MAX);
	glUniform2f(glGetUniformLocation(FramebufferProgram, "resolution"), width, height);




	//Get the textures in the Assets folder
	//Move to function : getTextures
	//-----------------
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
	//------------------

	SaveSystem SavingSystem;
	InputSystem InputHandler;

	SavingSystem.load("SCENE.ov");


	vec3 bg_rgb = SavingSystem.getVec3("BG_COLOR", vec3(0));
	float BackroundScreen[3];
	BackroundScreen[0] = bg_rgb.r;
	BackroundScreen[1] = bg_rgb.g;
	BackroundScreen[2] = bg_rgb.b;

	//function : retirive data
	//-------------
	int amount = SavingSystem.getInt("OBJ_AMOUNT", 3);
	for (int i = 0; i < amount; i++) {
		float posx, posy, scalex, scaley, angle, layer, restitution, density, statFric, dynaFric, velX, velY, tilex, tiley, cornerRadius;
		bool runtimeDraw, isStatic, isTrigger;
		glm::vec4 tint;
		int phys_layer = 0;
		std::string name, texture;

		name = SavingSystem.getString("OBJ" + std::to_string(i) + "_NAME", std::to_string(i).c_str());
		posx = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_POS_X", 0.0f);
		posy = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_POS_Y", 0.0f);
		scalex = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_SCA_X", 0.0f);
		scaley = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_SCA_Y", 0.0f);
		angle = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_ANGLE", 0.0f);
		texture = SavingSystem.getString("OBJ" + std::to_string(i) + "_TEXTURE", "");
		layer = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_LAYER", 0.0f);
		runtimeDraw = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_RUNDRAW", 0.0f);

		phys_layer = SavingSystem.getInt("OBJ" + std::to_string(i) + "_PHYSLAYER", 0);
		statFric = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_SFRIC", 0.6f);
		dynaFric = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_DFRIC", 0.4f);
		density = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_DENS", 0.5f);
		restitution = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_BOUNCE", 0.5f);
		isStatic = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_STATIC", 0.0f);
		isTrigger = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_TRIG", 0.0f);

		tilex = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_TTX", 0.0f);
		tiley = SavingSystem.getFloat("OBJ" + std::to_string(i) + "_TTY", 0.0f);

		tint.x = SavingSystem.getFloat("OBJ" + std::to_string(i) + "TINX", 0.0f);
		tint.y = SavingSystem.getFloat("OBJ" + std::to_string(i) + "TINY", 0.0f);
		tint.z = SavingSystem.getFloat("OBJ" + std::to_string(i) + "TINZ", 0.0f);
		cornerRadius = SavingSystem.getFloat("OBJ" + std::to_string(i) + "CR", 0.0f);

		int parent = SavingSystem.getInt("OBJ" + std::to_string(i) + "_PAREN", 0);
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
		obj.sceneAngle = angle;
		obj.texChar = texture;
		obj.layer = layer;
		obj.drawOnRuntime = runtimeDraw;

		obj.Body->layer = phys_layer;
		obj.Body->staticFric = statFric;
		obj.Body->dynamicFric = dynaFric;
		obj.Body->density = density;
		obj.Body->restitution = restitution;
		obj.Body->IsStatic(isStatic);
		obj.Body->IsTrigger(isTrigger);

		obj.TileX = tilex;
		obj.TileY = tiley;
		obj.tint = glm::vec4(tint.x, tint.y, tint.z, 1);
		obj.cornerRadius = cornerRadius;

		obj.tex = Texture((texture).c_str());
		obj.parent = parent;
		sceneObjects.push_back(obj);
	}





	//Declare the FrameBuffers.
	glEnable(GL_MULTISAMPLE);


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



	GLuint fboMouse;
	glGenFramebuffers(1, &fboMouse);
	glBindFramebuffer(GL_FRAMEBUFFER, fboMouse);

	GLuint textureColorBuffer;
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	
	
	//Engine Settings
	int FrameBufferMouseDetectedObject = -1;
	float ndcMouseX;
	float ndcMouseY;
	double mouseX;
	double mouseY;
	double beforeMouseX;
	double beforeMouseY;
	double beforeMouseXCam;
	double beforeMouseYCam;
	bool StartPhase = false;
	bool no_resize = true;
	bool no_move = true;
	bool mouseOverUI = false;
	Object blackbox = Object(verts, ind);
	Object scaleRight = Object(verts, ind);
	Object scaleUp = Object(verts, ind);
	Object scaleLeft = Object(verts, ind);
	Object scaleDown = Object(verts, ind);
	Texture EngineOVObjectIconGui("EngineAssets/ObjectIcon.png");
	Texture EngineOVCameraIconGui("EngineAssets/CameraIcon.png");
	Texture EngineOVTrashIconGui("EngineAssets/OvTrashIcon.png");
	Texture EngineOVScriptIconGui("EngineAssets/OvScriptIcon.png");
	Texture EngineOVRebuildIconGui("EngineAssets/OvRebuildIcon.png");
	Texture EngineOVRunIconGui("EngineAssets/OvRunIcon.png");
	Texture EngineOVBuildIconGui("EngineAssets/OvBuildIcon.png");
	Texture EngineOVMMSIconGui("EngineAssets/mmsicon.png");
	Texture EngineCuserIconGui("EngineAssets/Curser.png");
	Texture EngineWhiteIconGui("EngineAssets/white.png");
	Texture EngineLayerIconGui("EngineAssets/LayerIcon.png");
	Texture EngineBindIconGui("EngineAssets/Bind.png");
	Texture nulltex = Texture("");
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	int selectedObject = 0;
	int HighlightedObject = 0;
	float fov = 45;
	bool b_onpopupmenu = false;
	bool b_releasedUndo = true;
	const float fixed_timestep = 1.0f / 60.0f;
	FileFunction fileFunction;
	PhysicsWorld world(vec3(0, -35.0f, 0), 2);

	//Save function
	auto Save = [&]() {
		SavingSystem.save("OBJ_AMOUNT", (int)sceneObjects.size());

		for (int i = 0; i < sceneObjects.size(); ++i) {
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
			SavingSystem.save("OBJ" + std::to_string(i) + "_SFRIC", sceneObjects[i].Body->staticFric);
			SavingSystem.save("OBJ" + std::to_string(i) + "_DFRIC", sceneObjects[i].Body->dynamicFric);
			SavingSystem.save("OBJ" + std::to_string(i) + "_DENS", sceneObjects[i].Body->density);
			SavingSystem.save("OBJ" + std::to_string(i) + "_BOUNCE", sceneObjects[i].Body->restitution);
			SavingSystem.save("OBJ" + std::to_string(i) + "_STATIC", sceneObjects[i].Body->IsStatic());
			SavingSystem.save("OBJ" + std::to_string(i) + "_TRIG", sceneObjects[i].Body->IsTrigger());

			SavingSystem.save("OBJ" + std::to_string(i) + "_TTX", sceneObjects[i].TileX);
			SavingSystem.save("OBJ" + std::to_string(i) + "_TTY", sceneObjects[i].TileY);

			SavingSystem.save("OBJ" + std::to_string(i) + "TINX", sceneObjects[i].tint.x);
			SavingSystem.save("OBJ" + std::to_string(i) + "TINY", sceneObjects[i].tint.y);
			SavingSystem.save("OBJ" + std::to_string(i) + "TINZ", sceneObjects[i].tint.z);
			SavingSystem.save("OBJ" + std::to_string(i) + "CR", sceneObjects[i].cornerRadius);
			SavingSystem.save("OBJ" + std::to_string(i) + "_PAREN", sceneObjects[i].parent);
		}


		SavingSystem.save("BG_COLOR", vec3(BackroundScreen[0], BackroundScreen[1], BackroundScreen[2]));
		SavingSystem.saveToFile("SCENE.ov");

		Data.data = { float(vsync), float(msaa), BackroundScreen[0], BackroundScreen[1], BackroundScreen[2],
					   float(DrawFramebuffer), VigRadius, VigSoftness, FXAA_SPAN_MAX, FXAA_REDUCE_MIN, FXAA_REDUCE_MUL, CMX, CMY, float(Nearest) };

		Data.saveData();
	};
	glm::vec3 beforeMouseWorldPos;
	while (!glfwWindowShouldClose(window))
	{

		
		ImTextureID RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVTrashIconGui.ID));
		//Update definition shader values.
		glUniform1f(glGetUniformLocation(FramebufferProgram, "minEdgeContrast"), FXAA_REDUCE_MIN);
		glUniform1f(glGetUniformLocation(FramebufferProgram, "subPixelAliasing"), FXAA_REDUCE_MUL);
		glUniform1f(glGetUniformLocation(FramebufferProgram, "maximumEdgeDetection"), FXAA_SPAN_MAX);
		glUniform1f(glGetUniformLocation(FramebufferProgram, "radius"), VigRadius);
		glUniform1f(glGetUniformLocation(FramebufferProgram, "softness"), VigSoftness);


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
		if (sceneObjects.size() > 1 && selectedObject != -1) {
			ObjectCreator(window, sceneObjects[selectedObject]);
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
		b_onpopupmenu = false;
		if (ImGui::BeginPopup("Selected Object Settings"))
		{
			b_onpopupmenu = true;
			ObjectUI(window, HighlightedObject, EngineLayerIconGui);
			/*
			if (ImGui::CollapsingHeader(sceneObjects[selectedObject].name == "" ? std::to_string(selectedObject).c_str() : sceneObjects[selectedObject].name.c_str()))
			{
				ObjectUI(window, hoveredObject);
			}
			*/
			ImGui::EndPopup();
		}
		

		if (ImGui::IsMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
		{
			show_selected_pop = false;
		}
		

#pragma region Pre-Runtime
		//Engine
		//----------------------------------------------------------
		if (!build) {
			

			fov = scroll_offset;

			style.ItemSpacing.x = originalItemSpacing.x;
			style.ItemSpacing.y = originalItemSpacing.y;
			if (!StartPhase) {


				//dont need
				//------------
				camera.Position.x = 0;
				camera.Position.y = 0;
				//------------
				
				//sf::Listener::setGlobalVolume(0);
				//fov = 45;
				//fov = 32;
				std::string title = "OpenVision *(Universal Editor) ~ *" + ProjectName.string();
				glfwSetWindowTitle(window, title.c_str());
				for (size_t i = 0; i < sceneObjects.size(); i++)
				{

					sceneObjects[i].Body->LinearVelocity(glm::vec2(0, 0));
					sceneObjects[i].Body->AngularVelocity(0);

					if (sceneObjects[i].name == "MainCameraOvSTD") {
						sceneObjects[i].tex = EngineOVCameraIconGui;
					}

				}
				//sceneObjects[0].Body->GetCollider()->CalculateAABB();
				for (int i = 0; i < sceneObjects.size(); i++) {
					world.RemoveBody(sceneObjects[i].Body);
				}
				StartPhase = true;
			}
			mouseOverUI = false;

			if (!InputHandler.GetKey(GLFW_KEY_LEFT_CONTROL) || !InputHandler.GetKey(GLFW_KEY_Z)) {
				b_releasedUndo = true;
			}

			if (InputHandler.GetKey(GLFW_KEY_LEFT_CONTROL) && InputHandler.GetKey(GLFW_KEY_Z) && b_releasedUndo) {
				undoAction();
				b_releasedUndo = false;
			}

#pragma endregion Pre-Runtime

#pragma region UI
			
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
			if (ImGui::BeginMenu("Build"))
			{
				
				if (ImGui::MenuItem("Settings"))
				{
					OpenProjectSettings = true;
				}

				
				if (ImGui::MenuItem("Launch"))
				{
					//save
				}
				ImGui::EndMenu();
			}
			if (OpenProjectSettings) {
				popupProjectSettings();
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

			if (ImGui::BeginMenu("Install"))
			{
				popupforinstall();
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
			ImGui::Begin("Execute", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar);
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
				std::system("taskkill /F /IM Ovruntime.exe");

				Save();

				std::ofstream batchFile("temp.bat");

				if (batchFile.is_open())
				{
					batchFile << "@echo off" << std::endl;
					batchFile << "mkdir DEBUG_BUILD 2>nul /Q" << std::endl;
					batchFile << "echo D | xcopy /s /e /q /y Assets DEBUG_BUILD\\Assets 2>nul" << std::endl;
					batchFile << "echo. > DEBUG_BUILD\\ov.ov" << std::endl;
					batchFile << "copy /Y x64\\Release\\Vision_engine.exe DEBUG_BUILD\\Ovruntime.exe 2>nul" << std::endl;
					batchFile << "copy /Y Hanken - Light.ttf BuildGL\\Hanken - Light.ttf 2>nul" << std::endl;
					batchFile << "copy /Y OpenVisionIcon.png DEBUG_BUILD\\OpenVisionIcon.png 2>nul" << std::endl;
					batchFile << "copy /Y Scene.ov DEBUG_BUILD\\Scene.ov 2>nul" << std::endl;
					batchFile << "copy /Y runtimeconfig.ov DEBUG_BUILD\\runtimeconfig.ov 2>nul" << std::endl;
					batchFile << "copy /Y SETTINGS.ov DEBUG_BUILD\\SETTINGS.ov 2>nul" << std::endl;
					batchFile << "copy /Y SCRIPTS.ov DEBUG_BUILD\\SCRIPTS.ov 2>nul" << std::endl;
					batchFile << "copy /Y steam_api64.dll DEBUG_BUILD\\steam_api64.dll 2>nul" << std::endl;
					batchFile << "copy /Y DynaLL\\x64\\Release\\DynaLL.dll DEBUG_BUILD\\DynaLL.dll 2>nul" << std::endl;
					batchFile << "cd /D DEBUG_BUILD" << std::endl;
					batchFile << "start /B Ovruntime.exe" << std::endl;
					batchFile << "exit" << std::endl;

					batchFile.close();

					std::system("temp.bat");

					std::remove("temp.bat");
				}
			}


			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text("Run (Rebuild with run)");

				ImGui::EndTooltip();
			}
			ImGui::SameLine();
			RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVRebuildIconGui.ID));
			/*
			if (ImGui::ImageButton(RebuildimguiTextureID, ImVec2(30, 30)))
			{
				
				glfwSetWindowShouldClose(window,GLFW_TRUE);
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text("Re-build project");

				ImGui::EndTooltip();
			}
			*/

			ImGui::SameLine();
			RebuildimguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVBuildIconGui.ID));


			if (ImGui::ImageButton(RebuildimguiTextureID, ImVec2(30, 30)))
			{
				std::system("taskkill /F /IM Ovruntime.exe");

				Save();

				std::ofstream batchFile("temp.bat");

				if (batchFile.is_open())
				{
					
					batchFile << "@echo off" << std::endl;
					batchFile << "mkdir BuildGL 2>nul /Q" << std::endl;
					batchFile << "echo D | xcopy /s /e /q /y Assets BuildGL\\Assets 2>nul" << std::endl;
					batchFile << "echo. > BuildGL\\ov.ov" << std::endl;
					batchFile << "copy /Y x64\\Release\\Vision_engine.exe BuildGL\\Ovruntime.exe 2>nul" << std::endl;
					batchFile << "copy /Y Hanken - Light.ttf BuildGL\\Hanken - Light.ttf 2>nul" << std::endl;
					batchFile << "copy /Y OpenVisionIcon.png BuildGL\\OpenVisionIcon.png 2>nul" << std::endl;
					batchFile << "copy /Y Scene.ov BuildGL\\Scene.ov 2>nul" << std::endl;
					batchFile << "copy /Y runtimeconfig.ov BuildGL\\runtimeconfig.ov 2>nul" << std::endl;
					batchFile << "copy /Y SETTINGS.ov BuildGL\\SETTINGS.ov 2>nul" << std::endl;
					batchFile << "copy /Y SCRIPTS.ov BuildGL\\SCRIPTS.ov 2>nul" << std::endl;
					batchFile << "copy /Y steam_api64.dll DEBUG_BUILD\\steam_api64.dll 2>nul" << std::endl;
					batchFile << "copy /Y DynaLL\\x64\\Release\\DynaLL.dll BuildGL\\DynaLL.dll 2>nul" << std::endl;
					batchFile << "cd /D BuildGL" << std::endl;
					batchFile << "exit" << std::endl;

					batchFile.close();

					std::system("temp.bat");

					std::remove("temp.bat");
				}


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
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				

			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text("Exit program");

				ImGui::EndTooltip();
			}

			style.ButtonTextAlign = originalButtonTextAlign;
			style.FramePadding = originalFramePadding;



			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			ImGui::End();
			/*
			if (strcmp(con.input_buf, "devmode") == 0 && glfwGetKey(window, GLFW_KEY_ENTER))
			{
				con.log("Entering Devloper mode...");
			}
			*/
			con.Draw(no_resize, no_move);


			if (con.isConsoleHove)
			{
				mouseOverUI = true;
			}

			ImGui::End();
			ImGui::Begin("Sources", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar);
			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}
			if (ImGui::BeginTabBar("TabBar"))
			{
				if (ImGui::BeginTabItem("Object Settings"))
				{
					ObjectUI(window, selectedObject, EngineLayerIconGui);

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
							sceneObjects[selectedObject].tex = textures[k];
							sceneObjects[selectedObject].texChar = textures[k].FullImageFile;
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
						static const char* items[] = { "GL_NEAREST", "GL_LINEAR" };
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
			ImGui::Begin("Scripts", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar);
			if (ImGui::BeginTabBar("MainBar"))
			{
				if (ImGui::BeginTabItem("Scripts"))
				{
					ImGui::InputText("Script Name", scriptName, IM_ARRAYSIZE(scriptName));

					if (ImGui::Button("Add Script"))
					{
						std::ofstream outputFile("scripts.ov", std::ios::app);
						outputFile << scriptName << std::endl;
						outputFile.close();
						addOVscript(scriptName);
						memset(scriptName, 0, sizeof(scriptName));
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
							scriptName != "VBO")
						{
							std::string scriptname_str = scriptName;
							std::ifstream inputFile("SCRIPTS.ov");
							std::ofstream tempFile("temp.txt");
							std::string line;

							while (std::getline(inputFile, line))
							{
								if (line.find(scriptname_str) != std::string::npos)
								{
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
						}
					}

					if (ImGui::Button("Open Script"))
					{
						std::string command = "start DynaLL/" + std::string(scriptName) + ".h";
						system(command.c_str());
						memset(scriptName, 0, sizeof(scriptName));
					}

					if (ImGui::IsWindowHovered())
					{
						mouseOverUI = true;
					}

					// Ensure this End matches the BeginTabItem for "Scripts"
					ImGui::EndTabItem();

					ImGui::Begin("Scripts Select", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar);
					if (ImGui::IsWindowHovered())
					{
						mouseOverUI = true;
					}
					if (ImGui::Button("Script (gloabl ov script)"))
					{
						std::string command = "start DynaLL/Script.h";
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
					for (int row = 0; row < rowCount; ++row)
					{
						for (int col = 0; col < itemsPerRow; ++col)
						{
							int index = row * itemsPerRow + col;

							if (index < lines.size())
							{
								const auto& line = lines[index];

								ImGui::PushID(index);
								ImGui::BeginGroup();

								if (ImGui::ImageButton((ImTextureID)imguiTextureID, ImVec2(50, 50)))
								{
									std::string command = "start DynaLL/" + line + ".h";
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

					// Ensure this End matches the Begin for "Scripts Select"
					ImGui::End();
				}
				if (timeDiff >= fixed_timestep) {
					if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
					{
						animationKey++;
					}
				}
				if (ImGui::BeginTabItem("Animation"))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5)); // (horizontal, vertical) spacing
					// Ensure this End matches the BeginTabItem for "Animation"
					for (size_t i = 0; i < 1000; i++)
					{
						if (i == animationKey)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.7f, 1.0f));

						}
						else if (i % 2 == 0) {
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
						}
						else {
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.13f, 0.13f, 0.13f, 1.0f));

						}
						ImGui::Button(("K" + std::to_string(i)).c_str(), ImVec2(2.0f, 200.0f));
						ImGui::SameLine();
						
						ImGui::PopStyleColor();
					}
					ImGui::PopStyleVar();
					ImGui::EndTabItem();
				}

				// Ensure this End matches the BeginTabBar
				ImGui::EndTabBar();
			}

			ImGui::Begin("Window Control", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar);

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
			ImGui::Begin("Object Inspector", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar);

			if (ImGui::IsWindowHovered())
			{
				mouseOverUI = true;
			}

			if (ImGui::Button("Add object"))
			{

				sceneObjects.push_back(Object(verts, ind));
				sceneObjects[sceneObjects.size() - 1].name = "obj" + to_string(sceneObjects.size());
				AddObject action("obj" + to_string(sceneObjects.size()));
				undoStack.push(action);

			}
			{
				for (size_t i = 0; i < sceneObjects.size(); i++)
				{
					if (sceneObjects[i].parent != -1)
					{
						sceneObjects[i].parent_position = sceneObjects[sceneObjects[i].parent].position;

					}
					else {
						sceneObjects[i].parent_position = new glm::vec2(0, 0);
					}
					bool foundLastSelected = false;

					for (int i = sceneObjects.size() - 1; i >= 0; i--) {

						if (sceneObjects[i].selected) {

							if (!foundLastSelected) {
								sceneObjects[i].selected = true;
								selectedObject = i;
								foundLastSelected = true;
							}
							else {
								sceneObjects[i].selected = false;
							}
						}
					}


					if (sceneObjects[i].deleted == false) {
						if (i == selectedObject) {
							ImGui::SetNextItemOpen(true);
						}

						//ImGui::Separator();

						ImTextureID imguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVObjectIconGui.ID));
						if (sceneObjects[i].name != "MainCameraOvSTD") {
							imguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVObjectIconGui.ID));
						}
						else if (sceneObjects[i].name == "MainCameraOvSTD") {
							imguiTextureID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(EngineOVCameraIconGui.ID));
						}



						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						style.FramePadding = ImVec2(1.0f, 1.0f);
						style.ItemSpacing.x = 4.0f;

						ImVec2 imageSize(32, 32);
						if (sceneObjects[i].parent == -1) {
							ImGui::Image(imguiTextureID, imageSize);
						}
						
						

						if (ImGui::IsItemHovered())
						{
							if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && BindSelectedObject < 0)
							{
								BindSelectedObject = i;
							}
							if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && BindSelectedObject > 0)
							{
								if (i != BindSelectedObject) {
									sceneObjects[BindSelectedObject].parent = i;
								}
								BindSelectedObject = -1;

							}
							ImGui::BeginTooltip();
							if (sceneObjects[i].name != "MainCameraOvSTD") {
								ImGui::Text("An OpenVision regular object.");
							}
							else if (sceneObjects[i].name == "MainCameraOvSTD") {
								ImGui::Text("OpenVision's camera object (displayes the object's to the screen)");
							}
							ImGui::EndTooltip();
						}
						

						ImGui::SameLine();


						
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 0.0f));

						style.FramePadding.y = 11.0f;
						style.ItemSpacing.y = 0.0f;
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

						if (sceneObjects[i].parent == -1) {
							if (ImGui::CollapsingHeader(sceneObjects[i].name.c_str()))
							{
								style.FramePadding.y = originalButtonPadding;
								ObjectUI(window, i, EngineLayerIconGui);
								//Improve performence
								for (size_t j = 0; j < sceneObjects.size(); j++) {
									if (sceneObjects[j].parent == i)
									{
										ImGui::Separator();
										ObjectUI(window, j, EngineLayerIconGui);
										ImGui::Separator();
										

										
									}
								}
							}
						}
						style.ButtonTextAlign = originalButtonTextAlign;
						style.FramePadding = originalFramePadding;
						
						style.FramePadding.y = originalButtonPadding;



						//ImGui::Separator();

						if (!b_onpopupmenu) {
							glfwGetCursorPos(window, &mouseX, &mouseY);
						}
						ImGui::PopStyleVar();
						ImGui::PopStyleVar();
						//ImGui::PopStyleVar();
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
						float ndcMouseX = (2.0f * mouseX) / width - 1.0f;
						float ndcMouseY = 1.0f - (2.0f * mouseY) / height;  // Flip Y-axis for NDC
						glm::vec2 ndcMousePos(ndcMouseX, ndcMouseY);

						glm::vec4 viewport = glm::vec4(0, 0, width, height);

						glm::vec3 nearPoint = glm::unProject(glm::vec3(mouseX, height - mouseY, 0.0f),
							camera.getViewMatrix(),
							camera.getProjectionMatrix(fov, 0.1f, 100.0f),
							viewport);

						glm::vec3 farPoint = glm::unProject(glm::vec3(mouseX, height - mouseY, 1.0f),
							camera.getViewMatrix(),
							camera.getProjectionMatrix(fov, 0.1f, 100.0f),
							viewport);

						glm::vec3 rayDirection = glm::normalize(farPoint - nearPoint);

						float t = -nearPoint.z / rayDirection.z;
						glm::vec3 intersectionPoint = nearPoint + t * rayDirection;

						glm::vec3 mouseWorldDelta = intersectionPoint - beforeMouseWorldPos;

						float sensitivityFactor = 1.0f; 

						mouseWorldDelta *= sensitivityFactor;

						if (!scaling && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
							sceneObjects[selectedObject].position->x += mouseWorldDelta.x;
							sceneObjects[selectedObject].position->y += mouseWorldDelta.y;
						}

						beforeMouseWorldPos = intersectionPoint;

						//ndcMouseX *= widthW;
						//ndcMouseY *= heightW;
						int topIndex = -1;
						int topIndex2 = -1;

						float maxZIndex = -std::numeric_limits<float>::infinity();
						float maxZIndex2 = -std::numeric_limits<float>::infinity();

						if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
							CMX += (ndcMouseX - beforeMouseXCam);
							CMY += (ndcMouseY - beforeMouseYCam);

						}


						beforeMouseXCam = ndcMouseX;
						beforeMouseYCam = ndcMouseY;
						HighlightedObject = selectedObject;
						for (int i = 0; i < sceneObjects.size(); i++) {

							
							if (i == FrameBufferMouseDetectedObject && !mouseOverUI)
								
							{
								HighlightedObject = i;
								if (i != selectedObject) {
									glUseProgram(unlitProgram);
									glUniform4f(glGetUniformLocation(unlitProgram, "color"), 1.00, 0.48, 0.48, 1);

									glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
									glLineWidth(1.0f);
									if (timeDiff >= fixed_timestep) {
										sceneObjects[i].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
									}
									glUniform4f(glGetUniformLocation(unlitProgram, "color"), 1.00, 0.48, 0.48, 1);
									glLineWidth(1.0f);
								}
								
							}

							
						}

						for (int i = 0; i < sceneObjects.size(); i++) {

							
							if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && b_onpopupmenu == false &&
								FrameBufferMouseDetectedObject == i && !mouseOverUI)

							{
								if (sceneObjects[i].layer > maxZIndex) {
									topIndex = i;
									maxZIndex = sceneObjects[i].layer;
								}
							}

						}

						

						if (topIndex != -1 && !mouseOverUI) {
							if (!sceneObjects[topIndex].selected) {
								beforeMouseX = ndcMouseX;
								beforeMouseY = ndcMouseY;
							}
							else {


								float dx = ndcMouseX - beforeMouseX;
								float dy = ndcMouseY - beforeMouseY;
								if (scaling == false) {
									sceneObjects[topIndex].position->x += dx;
									sceneObjects[topIndex].position->y += dy;


								}

								beforeMouseX = ndcMouseX;
								beforeMouseY = ndcMouseY;
							}

							sceneObjects[topIndex].selected = true;
						}
						else {
							for (int i = 0; i < sceneObjects.size(); i++) {
								sceneObjects[i].selected = false;
							}
						}



						glUseProgram(unlitProgram);
						glUniform4f(glGetUniformLocation(unlitProgram, "color"), 0, 0.90, 0, 1);
						*scaleRight.angle = *sceneObjects[selectedObject].angle;
						scaleRight.position->x = sceneObjects[selectedObject].position->x + cos(*scaleRight.angle / 57.295779513082320876798154814105) * (sceneObjects[selectedObject].scale->x / 2.0);
						scaleRight.position->y = sceneObjects[selectedObject].position->y + sin(*scaleRight.angle / 57.295779513082320876798154814105) * (sceneObjects[selectedObject].scale->x / 2.0);
						*scaleRight.scale = glm::vec2(0.5f, sceneObjects[selectedObject].scale->y);
						
						*scaleUp.angle = *scaleRight.angle + 90;
						scaleUp.position->x = sceneObjects[selectedObject].position->x + cos(*scaleUp.angle / 57.295779513082320876798154814105) * (sceneObjects[selectedObject].scale->y / 2.0);
						scaleUp.position->y = sceneObjects[selectedObject].position->y + sin(*scaleUp.angle / 57.295779513082320876798154814105) * (sceneObjects[selectedObject].scale->y / 2.0);
						*scaleUp.angle = *scaleRight.angle ;
						*scaleUp.scale = glm::vec2(sceneObjects[selectedObject].scale->x, 0.5f);

						if (FrameBufferMouseDetectedObject == 255 * 256 * 256 + 255 * 256 + 255 && !mouseOverUI) {

							scaleRight.Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
							if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && b_onpopupmenu == false) {
								ScaleXBool = true;
								scaling = true;
								initialMouseX = ndcMouseX;
								initialScaleX = sceneObjects[selectedObject].scale->x;
							} 
						}

						if (FrameBufferMouseDetectedObject == 254 * 256 * 256 + 254 * 256 + 254 && !mouseOverUI) {

							scaleUp.Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
							if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && b_onpopupmenu == false) {
								ScaleXBool = false;
								scaling = true;
								initialMouseX = ndcMouseY;
								initialScaleX = sceneObjects[selectedObject].scale->y;
							}
						}


						if (scaling == true) {

							if (ScaleXBool) {
								scaleRight.Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
								float deltaX = ndcMouseX - initialMouseX;
								sceneObjects[selectedObject].scale->x = initialScaleX - deltaX;
							}
							else {
								scaleUp.Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
								float deltaX = ndcMouseY - initialMouseX;
								sceneObjects[selectedObject].scale->y = initialScaleX - deltaX;
							}
						}

						if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
							
							scaling = false;
						}
						glUseProgram(unlitProgram);
						glUniform4f(glGetUniformLocation(unlitProgram, "color"), 1.00, 0.42, 0.24, 1);

						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glLineWidth(2.0f);

						sceneObjects[selectedObject].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);

						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



						//Drawing
						sceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);



						
					}
				}
			}


		} //---------------------------------------------------

		
		Object RuntimeCam = *OV::SearchObjectByName("MainCameraOvSTD", sceneObjects);
		//blackbox.tex = nulltex;
		blackbox.layer = 100;
		blackbox.tint = glm::vec4(0, 0, 0, 1);
		float offsetX = CMX;
		float offsetY = CMY;
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(RuntimeCam.position->x + offsetX, ((-36 / 1.5) / 1.5) + RuntimeCam.position->y + offsetY), glm::vec2(59, 0.5));
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(RuntimeCam.position->x + offsetX, ((36 / 1.5) / 1.5) + RuntimeCam.position->y + offsetY), glm::vec2(59, 0.5));



		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2((61.7 / 1.445 / 1.5) + RuntimeCam.position->x + offsetX, RuntimeCam.position->y + offsetY), glm::vec2(0.5, 33.8));
		blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2((-61.7 / 1.445 / 1.5) + RuntimeCam.position->x + offsetX, RuntimeCam.position->y + offsetY), glm::vec2(0.5, 33.8));

		blackbox.tex = EngineWhiteIconGui;

		camera.Position.x = RuntimeCam.position->x + offsetX;
		camera.Position.y = RuntimeCam.position->y + offsetY;


		if (DrawFramebuffer) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glUseProgram(FramebufferProgram);
			glBindVertexArray(rectVAO);
			glDisable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D, framebufferTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, fboMouse);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mouse Detecting
		glUseProgram(MouseDetectionProgram);
		for (int i = 0; i < sceneObjects.size(); i++) {
			if (i != 255 || i != 254) {
				float redValue = float((i / (256 * 256)) % 256) / 255.0f; // High byte
				float greenValue = float((i / 256) % 256) / 255.0f; // Mid byte
				float blueValue = float(i % 256) / 255.0f; // Low byte

				glUniform3f(glGetUniformLocation(MouseDetectionProgram, "color"), redValue, greenValue, blueValue);
				sceneObjects[i].Draw(window, MouseDetectionProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
			}
		}

		glUniform3f(glGetUniformLocation(MouseDetectionProgram, "color"), 1, 1, 1);
		scaleRight.Draw(window, MouseDetectionProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
		glUniform3f(glGetUniformLocation(MouseDetectionProgram, "color"), float(254) / 255, float(254) / 255, float(254) / 255);

		scaleUp.Draw(window, MouseDetectionProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		
		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		
		glReadBuffer(GL_FRONT); 
		unsigned char pixel[3];
		glReadPixels((int)xpos, windowHeight - (int)ypos, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

		//DETECT COLORMOUSE
		FrameBufferMouseDetectedObject = pixel[0] * 256 * 256 + pixel[1] * 256 + pixel[2];

		//std::cout << "Object index under mouse: " << FrameBufferMouseDetectedObject << std::endl;
		/*
		std::cout << "Color under mouse: R=" << (int)pixel[0]
			<< " G=" << (int)pixel[1]
			<< " B=" << (int)pixel[2] << std::endl;
		*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		//blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(PresceneObjects[selectedObject].position->x + PresceneObjects[selectedObject].scale->x / 2 - CMX, PresceneObjects[selectedObject].scenePosition.y + CMY), glm::vec2(2.5, 2.5));


		/*
		if (!mouseOverUI) {
			blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(ndcMouseX, ndcMouseY), glm::vec2(2.5, 2.5));
		}
		*/

#pragma endregion Mouse-Detection

		//blackbox.tex = CenterDot;
		//blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(CMX,CMY), glm::vec2(5, 5));
#pragma region Runtime
		//Runtime class
		//-------------------------------
		if (build) {
			if (StartPhase)
			{
				SaveObjectsToFile("runtimeconfig.ov");
				ScriptStart();
				//sf::Listener::setGlobalVolume(100);


				for (int i = 0; i < sceneObjects.size(); i++) {
					world.AddBody(sceneObjects[i].Body);
				}



				std::cout << ObjectsSize() - sceneObjects.size();
				for (size_t i = 0; i < (ObjectsSize() - (sceneObjects.size() - i)); i++)
				{
					sceneObjects.push_back(Object(verts, ind));
					std::cout << i;
				}
				
				
				
				
				//std::cout << OVObjects.size();
				/*
				for (size_t i = 0; i < OVObjects.size(); i++)
				{
					std::cout << OVObjects[i].x;
				}
				*/
				//OVObjects = sceneObjects;?
				//std::cout << ObjectsSize() + "Objects size" << std::endl;
				//std::cout << sceneObjects.size() << std::endl;

				

				/*
				for (size_t i = 0; i < ObjectsSize(); i++)
				{

					if (GetSharedVarX(i).x == 90)
					{
						std::cout << "HI";
					}
					//sceneObjects[i].position->x = GetSharedVarX(i).x;
					//sceneObjects[i].position->y = GetSharedVarX(i).y;
					//sceneObjects[i].scale->y = GetSharedVarX(i).scale_y;
					//sceneObjects[i].scale->x = GetSharedVarX(i).scale_x;
				}
				*/
				/*
				std::string input = "90, 9, 12, 67";
				std::vector<float> result = splitStringToFloats(input);

				// Print the float values
				for (size_t i = 0; i < sceneObjects.size(); ++i) {
					sceneObjects[i].position->x = result[i];

				}

				*/

				con.CLEAR_CONSOLE();
				fov = 22.45;
				script.Start();
				//sharedVar = 15;
				


				//(sharedObject.x, sharedObject.y)
				//std::vector<int> sharedArray = GetShared(); 
				//.log(sharedArray[0]);

				

				//std::cout << sharedVar;
				StartPhase = false;
			}


			script.Update();

			/*
			std::vector<Ov_Object> obj = { Ov_Object{10,10,10,10, 10} };
			GetSharedObject(obj);

			//std::cout << obj[0].x;
			/*
			Ov_Object sharedObject = GetSharedObject();
			glm::vec2 objp(sharedObject.x, sharedObject.y);
			glm::vec2 objs(sharedObject.x, sharedObject.y);
			sceneObjects[sharedObject.where].position = &objp;
			sceneObjects[sharedObject.where].scale = &objs;
			*/

			/*
			vector<vec2> vertices = dynamic_cast<PolygonCollider&>(*sceneObjects[0].Body->GetCollider()).GetTransformedVertices();
			vector<vec3> vertices3 = vector<vec3>();
			for (size_t i = 0; i < vertices.size(); i++)
			{
				vertices3.push_back(vec3(vertices[i], 0));
				cout << "vertex:" << glm::to_string(vertices3[i]) << endl;
			}
			sceneObjects[0].DrawCustomVertices(window, shaderProgram, camera, vertices3);
			*/

			if (timeDiff >= fixed_timestep) {
				std::string FPS = std::to_string((1.0 / timeDiff) * counter);
				std::string newTitle = ProjectName.string() + " ~" + FPS + "FPS";
				glfwSetWindowTitle(window, newTitle.c_str());
				prevTime = crntTime;
				counter = 0;
				OVObjects[1].acc = true;
				if (glfwGetKey(window, GLFW_KEY_W)) {
					OVObjects[1].y += 0.4f;
				}
				if (glfwGetKey(window, GLFW_KEY_S)) {
					OVObjects[1].y -= 0.4f;
				}
				if (glfwGetKey(window, GLFW_KEY_D)) {
					OVObjects[1].x += 0.4f;
				}
				if (glfwGetKey(window, GLFW_KEY_A)) {
					OVObjects[1].x -= 0.4f;
				}
				for (size_t i = 0; i < sceneObjects.size(); i++)
				{
					if (GetSharedVarX(i).acc == true) {
						sceneObjects[i].position->x = GetSharedVarX(i).x;
						sceneObjects[i].position->y = GetSharedVarX(i).y;
						sceneObjects[i].scale->x = GetSharedVarX(i).scale_x;
						sceneObjects[i].scale->y = GetSharedVarX(i).scale_y;
					}
				}
				
				ScriptUpdate();
				


				world.Step(timeDiff / 2);

			}
			if (glfwWindowShouldClose(window))
			{
				script.Exit();
				ScriptExit();
			}
			//OVObjects[1].scale_x = 100;

			for (size_t i = 0; i < sceneObjects.size(); i++)
			{




				if (!sceneObjects[i].deleted) {
					glLineWidth(0.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glUseProgram(unlitProgram);
					glUniform4f(glGetUniformLocation(unlitProgram, "color"), sceneObjects[i].OutlineColor.x, sceneObjects[i].OutlineColor.y, sceneObjects[i].OutlineColor.z, 1);
					glLineWidth(sceneObjects[i].outlineWidth);
					if (sceneObjects[i].outlineWidth > 0) {
						sceneObjects[i].Draw(window, unlitProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
					}
					glLineWidth(0.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					if (sceneObjects[i].drawOnRuntime == true) {
						sceneObjects[i].Draw(window, shaderProgram, camera, glm::vec3(0, 0, 1), CMX, CMY, Nearest);
					}
				}
			}

		}
		//-------------------------------------------

#pragma endregion Runtime
	//Escape Class
	//---------------------------

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (BindSelectedObject > 0) {
			blackbox.tex = EngineBindIconGui;
			blackbox.DrawTMP(window, shaderProgram, camera, glm::vec2(ndcMouseX, ndcMouseY), glm::vec2(4, 4));
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
		InputHandler.Update(window);


		//find better way to detect build
		//------------------
		build = fileFunction.fileExists("ov.ov");
		//------------------

	}
	if (!build) {
		Save();
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glfwDestroyWindow(window);
	glfwTerminate();

	//-----------------------------

	//std::system("taskkill /f /im python.exe");
	//system("Vision_engine.exe");
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
