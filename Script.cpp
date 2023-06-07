#include"Script.h"
#include<filesystem>
glm::vec2 getImageAspectRatio(const char* filename) {
    int width, height, channels;
    stbi_uc* imageData = stbi_load(filename, &width, &height, &channels, 0);

    if (imageData != nullptr) {
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        stbi_image_free(imageData);
        return glm::vec2(aspectRatio, 1.0f);
    }
    else {
        std::cout << "Failed to load image." << std::endl;
        return glm::vec2(0.0f);
    }
}
std::vector<Texture> textures;

void Script::Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects, Camera& camera) {



}   
void Script::Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects, Camera& camera) {


}
