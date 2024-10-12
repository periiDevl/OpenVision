#include "OV.h"


glm::vec2 rattio(16, 9);

int width = 95 * rattio.x;
const unsigned int height = 95 * rattio.y;
std::vector<Object> sceneObjects;
Console con;
PhysicsWorld world(vec3(0, 55.0f, 0), 10);
Camera camera(width, height, glm::vec3(0.0f, 0.0f, 80.0f));
//InputSystem InputHandler(); // It needs a GLFW Window
