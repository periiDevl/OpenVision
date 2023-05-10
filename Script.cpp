#include"Script.h"
void Script::Start(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	for (int i = 0; i < sceneObjects.size(); i++) {
		world.AddBody(sceneObjects[i].Body);
	}
	sceneObjects[0].Body->velocity = vec2(5, 0);
	sceneObjects[1].Body->isStatic = true;
}
void Script::Update(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	//float horizontal = 0;
	//if (glfwGetKey(window, GLFW_KEY_D))
	//	horizontal = 1;
	//if (glfwGetKey(window, GLFW_KEY_A))
	//	horizontal = -1;
	//
	//sceneObjects[0].Body->velocity.x = horizontal * speed;
	//
	//float vertical = 0;
	//if (glfwGetKey(window, GLFW_KEY_S))
	//	vertical = 1;
	//if (glfwGetKey(window, GLFW_KEY_W))
	//	vertical = -1;
	//
	//sceneObjects[0].Body->velocity.y = vertical * speed;


	for (size_t i = 0; i < sceneObjects.size(); i++)
	{
		cout << "i " << i << "  fric:" << sceneObjects[i].Body->friction;
		if (glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT))
			sceneObjects[i].Body->friction = 1.0f;
		else if (glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT))
			sceneObjects[i].Body->friction = 0;
		cout << "after " << i << "  fric:" << sceneObjects[i].Body->friction;
	}
}
