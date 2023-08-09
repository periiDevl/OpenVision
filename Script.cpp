#include"Script.h"
#include<filesystem>

Object* m;
void Script::Start() {

	m = OV::SearchObjectByName("F", sceneObjects);
    OV::PlaySound("Overspeed Warning.wav");

}   
void Script::Update() {
    OV::renderText("Hello itay!", "Hanken-Light.ttf", 0, 0, 50.0f, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    std::string str = typeid(*this).name();
    
    if (InputHandler.GetKeyDown(GLFW_KEY_SPACE))
    {
        *m->angle = *m->angle + 5;
        OV::PlaySound("Overspeed Warning.wav");
    }

}
