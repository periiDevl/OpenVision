#include"Script.h"
float speed = 10;
Object* player = nullptr;
Object* ground = nullptr;
Object* ground2 = nullptr;

void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {



    player = OV::SearchObjectByName("Player", sceneObjects);
    ground = OV::SearchObjectByName("Ground", sceneObjects);
    ground2 = OV::SearchObjectByName("Ground 2", sceneObjects);    
}   
void Script::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {

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

    if (BoundingAABB(*ground->Body->GetCollider(), *player->Body->GetCollider()) || BoundingAABB(*ground2->Body->GetCollider(), *player->Body->GetCollider())) {
        if (Input.GetKey(GLFW_KEY_SPACE))
            player->Body->velocity.y = -10;
    }
}
