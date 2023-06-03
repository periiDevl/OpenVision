#include"Script.h"
float speed = 10;
Object* player = nullptr;
Object* ground_detection = nullptr;
bool jumping;

void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {



    player = OV::SearchObjectByName("Player", sceneObjects);
    ground_detection = OV::SearchObjectByName("GroundDetection", sceneObjects);

}   
void Script::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects) {
    *ground_detection->position = *player->position + vec2(0, 1);

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

    if (world.TouchingLayer(ground_detection->Body, 1)){
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

    if (world.TouchingLayer(player->Body, 2))
    {
        OV::SetTexture("Assets/player.png", *player);
    }
}
