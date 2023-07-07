#include"Script.h"
#include<filesystem>


std::vector<Texture> textures;
float fl;
void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects, Camera& camera) {


    //OV::PlaySound("Overspeed Warning.wav");

}   
void Script::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects, Camera& camera) {
    OV::renderText("Hello itay!", "Hanken-Light.ttf", 0, 0, 50.0f, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    std::string str = typeid(*this).name();
    
    if (Input.GetKeyDown(GLFW_KEY_SPACE))
    {
        //OV::PlaySound("Overspeed Warning.wav");
    }

}
