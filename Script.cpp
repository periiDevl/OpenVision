#include"Script.h"
#include<filesystem>


std::vector<Texture> textures;
float fl;
void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects, Camera& camera) {


    OV::PlaySound("Overspeed Warning.wav");

}   
void Script::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects, Camera& camera) {
    std::string str = typeid(*this).name();
    
    if (Input.GetKeyDown(GLFW_KEY_SPACE))
    {
        OV::PlaySound("Overspeed Warning.wav");
    }

}
