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

    obj.transform->position.x = 1;
    obj.transform->position.y = 0;
    obj.transform->scale.x = 1;
    obj.transform->scale.y = 1;

    TextureRenderer& renderer = *obj.addComponent<TextureRenderer>();
    //itay make the component work
    renderer.init();

    Texture EngineOVObjectIconGui("Assets/background.png");
    renderer.tex = EngineOVObjectIconGui;

    updateFPSTimer.start();
    int frames = 0;
    int targetFPS = 200;
    long long targetFrameTimeNs = static_cast<long long>(1e9 / targetFPS);
    long long sleepThresholdNs = targetFrameTimeNs;

    Timer inputTimer;
    Shader classic_shader(vertexShaderSource, fragmentShaderSource);

    Camera camera1(window.width, window.height, glm::vec3(0.0f, 0.0f, 0.2f));

    glm::vec2 mousePre = window.mouseAsWorldPosition(camera1);
    glm::vec2 mouseCur = window.mouseAsWorldPosition(camera1);

    while (window.windowRunning())
    {
        fpsTimer.start();
        frames++;

        window.clear();
        camera1.updateMatrix(0.1f, 100.0f);
        classic_shader.Activate();
        renderer.draw(window.getWindow(), classic_shader.ID, camera1, glm::vec3(0, 0, 1));

        mousePre = mouseCur;
        mouseCur = window.mouseAsWorldPosition(camera1);

        if (renderer.checkMouseBoundry(mouseCur, window.width, window.height))
        {
            if (InputSystem::getHold(Inputs::MouseRight))
            {
                obj.transform->position += (mouseCur - mousePre);
            }
        }
        window.update();

        // Limit FPS
        while (fpsTimer.getElapsedNanoSeconds() < sleepThresholdNs);
    }

    return 0; // Return 0 for successful execution
}
