#include"Script.h"
float speed = 10;
Object* player = nullptr;
Object* ground = nullptr;
Object* ground2 = nullptr;
bool jumping;

void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {



    player = OV::SearchObjectByName("Player", sceneObjects);
    ground = OV::SearchObjectByName("Ground", sceneObjects);
    ground2 = OV::SearchObjectByName("Ground 2", sceneObjects);    
    ground->Body->layer = 1;
    ground2->Body->layer = 1;
    world.UpdateLayerBodies();
    /// ADD GUI FOR IT TMRW
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

    if (world.TouchingLayer(player->Body, 1)){//BoundingAABB(*ground->Body->GetCollider(), *player->Body->GetCollider()) || BoundingAABB(*ground2->Body->GetCollider(), *player->Body->GetCollider())) {
        if (Input.GetKey(GLFW_KEY_SPACE)) {
            player->Body->velocity.y = -30;
            jumping = true;
        }
    }
    if (player->Body->velocity.y >= 0)
        jumping = false;

    if (!Input.GetKey(GLFW_KEY_SPACE) && jumping) {
        jumping = false;
        player->Body->velocity.y /= 2;
    }
}
