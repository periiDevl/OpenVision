#include"Script.h"
void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	for (int i = 0; i < sceneObjects.size(); i++) {
		world.AddBody(sceneObjects[i].Body);
		if (sceneObjects[i].name == "Player")
			player = &sceneObjects[i];

		if (sceneObjects[i].name == "Ground")
			ground = &sceneObjects[i];
		if (sceneObjects[i].name == "Ground 2")
			ground2 = &sceneObjects[i];

		if (sceneObjects[i].name == "FloorDetection")
			ground_detection = &sceneObjects[i];
	}
	
	player->Body->velocity = vec2(5, 0);
	player->Body->restitution = 0.0f;
	ground2->Body->isStatic = true;
	ground->Body->isStatic = true;
	ground_detection->Body->isStatic = true;
	ground_detection->Body->isTrigger = true;
	
	SavingSystem = SaveSystem();
	SavingSystem.load("Scene.ov");
}
void Script::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
	*ground_detection->position = *player->position - vec2(0, player->scale->y*-.24f);
	
	float horizontal = 0;
	if (Input.GetKey(GLFW_KEY_D)) {
		player->scale->x = std::abs(player->scale->x);
		horizontal = 1;
	}
	if (Input.GetKey(GLFW_KEY_A)) {
		player->scale->x = -std::abs(player->scale->x);
		horizontal = -1;
	}
	player->Body->velocity.x = horizontal * speed;

	if (BoundingAABB(*ground->Body->GetCollider(), *ground_detection->Body->GetCollider()) || BoundingAABB(*ground2->Body->GetCollider(), *ground_detection->Body->GetCollider())) {
		if (Input.GetKeyDown(GLFW_KEY_SPACE))
			player->Body->velocity.y = -10;
	}
}
