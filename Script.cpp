#include"Script.h"
void Script::Start(Console& ovcon, std::vector<Object>& sceneObjects)
{
	ovcon.log("Hello World!");
	sceneObjects[0].position.x = 10;
}
void Script::Update(Console& ovcon, std::vector<Object>& sceneObjects)
{

}