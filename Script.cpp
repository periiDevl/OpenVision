#include"Script.h"
float speed = 10;
Object* player = nullptr;
Object* ground = nullptr;
Object* ground2 = nullptr;
Object* ground_detection = nullptr;

void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {



    player = OV::SearchObjectByName("Player", sceneObjects);
    ground = OV::SearchObjectByName("Ground", sceneObjects);
    ground2 = OV::SearchObjectByName("Ground 2", sceneObjects);
    ground_detection = OV::SearchObjectByName("FloorDetection", sceneObjects);


    player->Body->velocity = vec2(5, 0);
    player->Body->restitution = 0.0f;
    ground2->Body->isStatic = true;
    ground->Body->isStatic = true;
    ground_detection->Body->isStatic = true;
    ground_detection->Body->isTrigger = true;


}
void Script::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
    *ground_detection->position = *player->position - vec2(0, player->scale->y* - .48f);

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
        if (Input.GetKey(GLFW_KEY_SPACE))
            player->Body->velocity.y = -10;
    }
}
