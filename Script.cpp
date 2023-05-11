#include"Script.h"
void Script::Start(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	for (int i = 0; i < sceneObjects.size(); i++) {
		world.AddBody(sceneObjects[i].Body);
		if (sceneObjects[i].name == "Player")
			player = &sceneObjects[i];

		if (sceneObjects[i].name == "Ground")
			ground = &sceneObjects[i];

		if (sceneObjects[i].name == "FloorDetection")
			ground_detection = &sceneObjects[i];
	}
	player->Body->velocity = vec2(5, 0);
	player->Body->restitution = 0.0f;
	ground->Body->isStatic = true;
	ground_detection->Body->isStatic = true;
	ground_detection->Body->isTrigger = true;

	SavingSystem = SaveSystem();
	SavingSystem.load("Scene.ov");
}
void Script::Update(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	*ground_detection->position = *player->position - vec2(0, -2.4f);
	
	float horizontal = 0;
	if (glfwGetKey(window, GLFW_KEY_D)) {
		horizontal = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		horizontal = -1;
	}
	player->Body->velocity.x = horizontal * speed;

	if (BoundingAABB(*ground->Body->GetCollider(), *ground_detection->Body->GetCollider())) {
		if (glfwGetKey(window, GLFW_KEY_SPACE))
			player->Body->velocity.y = -10;
	}
}
