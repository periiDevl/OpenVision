
#include"EpicNewScript.h"
void EpicNewScript::Start(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects)
{
    ovcon.log("Hello World!");
}
void EpicNewScript::Update(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects)
{

    ovcon.log("Hello Update!");
}
    