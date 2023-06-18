#include"Script.h"
#include<filesystem>


std::vector<Texture> textures;

void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects, Camera& camera) {
    std::string str = typeid(*this).name();
    float fl = OV::PublicFloat(0, str);


    


}   
void Script::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects, Camera& camera) {


}
