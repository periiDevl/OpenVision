#include"Script.h"
void Script::Start(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	for (int i = 0; i < sceneObjects.size(); i++) {
		world.AddBody(sceneObjects[i].Body);
	}
	sceneObjects[0].Body->velocity = vec2(5, 0);
	sceneObjects[0].Body->restitution = 0.0f;
	sceneObjects[1].Body->isStatic = true;
	sceneObjects[2].Body->isStatic = true;
	sceneObjects[2].Body->isTrigger = true;

	SavingSystem = SaveSystem();
	SavingSystem.load("Scene.ov");
}
void Script::Update(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	*sceneObjects[2].position = *sceneObjects[0].position - vec2(0, -2.4f);
	float horizontal = 0;
	if (glfwGetKey(window, GLFW_KEY_D)) {
		SavingSystem.save("direction", std::string("Right"));
		SavingSystem.saveToFile("Scene.ov");

		horizontal = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		SavingSystem.save("direction", std::string("Left"));
		SavingSystem.saveToFile("Scene.ov");

		horizontal = -1;
	}
	sceneObjects[0].Body->velocity.x = horizontal * speed;

	if (BoundingAABB(*sceneObjects[1].Body->GetCollider(), *sceneObjects[2].Body->GetCollider())) {
		if (glfwGetKey(window, GLFW_KEY_SPACE))
			sceneObjects[0].Body->velocity.y = -10;
	}
}
