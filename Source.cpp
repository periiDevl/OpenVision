#include <iostream>
#include <chrono>
#include "TextureRenderer.h"
#include "Window.h"
#include "InputSystem.h"
#include "Timer.h"
#include <thread>
#include "OV.h"
#include "Settings.h"
#include "Object.h"
#include "Shader.h"
#include "OVLIB.h"
// Function declarations

int main()
{
    Timer fpsTimer;
    Timer updateFPSTimer;
    Window window;
    GameObject obj;

    // Initialize offsets and sizes
    float xOffset = -((window.width * 0.0025)) + 2;
    float yOffset = ((window.height * 0.0025)) - 2;
    float xSizeOff = ((window.width * 0.0025)) - 2;
    float ySizeOff = ((window.height * 0.0025)) - 2;

    obj.transform->position.x = 0;
    obj.transform->position.y = 0;
    obj.transform->scale.x = 1;
    obj.transform->scale.y = 1;

    TextureRenderer& renderer = *obj.addComponent<TextureRenderer>();
    renderer.vertices = verts; // Assuming verts is defined elsewhere
    renderer.indices = ind;     // Assuming ind is defined elsewhere
    renderer.init();

    Texture EngineOVObjectIconGui("Assets/background.png");
    renderer.tex = EngineOVObjectIconGui;

    updateFPSTimer.start();
    int frames = 0;
    int targetFPS = 200;
    long long targetFrameTimeNs = static_cast<long long>(1e9 / targetFPS);
    long long sleepThresholdNs = targetFrameTimeNs;

    Timer inputTimer;
    Shader classic_shader(vertexShaderSource, fragmentShaderSource); // Assuming these are defined

    Camera camera1(window.width, window.height, glm::vec3(0.0f, 0.0f, 0.2f));
    while (window.windowRunning())
    {
        fpsTimer.start();
        frames++;

        window.clear();
        camera1.updateMatrix(0.1f, 100.0f);
        classic_shader.Activate();
        renderer.draw(window.getWindow(), classic_shader.ID, camera1, glm::vec3(0, 0, 1));

        if (updateFPSTimer.getElapsedSeconds() > 1)
        {
            window.setTitle("FPS: ~" + std::to_string(frames));
            updateFPSTimer.start();
            frames = 0;
        }

        // Input handling
        if (InputSystem::getDown(Inputs::KeyD))
        {
            int axis = InputSystem::getAxis(Inputs::KeyLeft, Inputs::KeyRight);
            std::cout << "value: " << axis << "\n";
        }

        // Get mouse position
        glm::vec2 value = InputSystem::getMousePosition();

        // Normalize mouse position to [-1, 1]
        float normalizedMouseX = (value.x / window.width) * 2 - 1; // Convert to NDC (-1 to 1)
        float normalizedMouseY = -((value.y / window.height) * 2 - 1); // Invert y for NDC

        // Dynamic scaling based on window size
        float scaleFactorX = window.width / 200.0f;  // Base scaling factor for a width of 800
        float scaleFactorY = window.height / 200.0f; // Base scaling factor for a height of 800





        // Scale the normalized coordinates dynamically
        float scaledMouseX = normalizedMouseX * scaleFactorX; // Dynamically adjust based on window width
        float scaledMouseY = normalizedMouseY * scaleFactorY; // Dynamically adjust based on window height

        // Update the object's position to center it at the mouse position
        //obj.transform->position.x = scaledMouseX;  // Adjust x position
        //obj.transform->position.y = scaledMouseY; // Adjust y position
        if (scaledMouseX < obj.transform->position.x + obj.transform->scale.x * 2 && scaledMouseX > obj.transform->position.x - obj.transform->scale.x * 2 
            && scaledMouseY < obj.transform->position.y + obj.transform->scale.y * 2
            && scaledMouseY > obj.transform->position.y - obj.transform->scale.y * 2)
        {
            printf("Inside");
        }

        std::cout << "Mouse Position: " << scaledMouseX << ", " << scaledMouseY << '\n';

        // Cursor mode handling
        if (InputSystem::getDown(Inputs::KeyE))
        {
            InputSystem::setCursorMode(InputCursor::Disabled);
        }

        if (InputSystem::getDown(Inputs::KeyW))
        {
            InputSystem::setCursorMode(InputCursor::Hidden);
        }

        if (InputSystem::getDown(Inputs::KeyQ))
        {
            InputSystem::setCursorMode(InputCursor::Normal);
        }

        if (InputSystem::getHold(Inputs::MouseRight))
        {
            inputTimer.start();
        }

        if (InputSystem::getDown(Inputs::MouseLeft))
        {
            std::cout << "Pressed mouse left for: " << inputTimer.getElapsedMilliSeconds() << " ms \n";
        }

        window.update();

        // Limit FPS
        while (fpsTimer.getElapsedNanoSeconds() < sleepThresholdNs);
    }

    return 0; // Return 0 for successful execution
}
