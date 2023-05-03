#include"Script.h"
void Script::Start(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	for (int i = 0; i < sceneObjects.size(); i++) {
		world.AddBody(sceneObjects[i].Body);
		sceneObjects[i].Body->velocity = vec2(1, 0);
	}
}
void Script::Update(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		world.SetGravity(-world.GetGravity());
	}
}
