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
#include "CircleCollider.h"
#include "GJK.h"
#include "EPA.h"

#include "OverDepth.h"
// Function declarations

int main()
{
    Timer fpsTimer;
    Timer updateFPSTimer;
    Camera camera1(glm::vec3(0.0f, 0.0f, 0.2f), 1280, 800);
    Window window(camera1);

    GameObject obj;
    GameObject obj2;

    obj.transform->position = { -1, 0 };
    obj.transform->scale = { 1, 1 };
    obj2.transform->position = { 1, 0 };
    obj2.transform->scale = { 1, 1 };

    TextureRenderer& renderer = *obj.addComponent<TextureRenderer>();
    CircleCollider coll(obj.transform->position, 0.5f);

    TextureRenderer& renderer2 = *obj2.addComponent<TextureRenderer>();
    CircleCollider coll2(obj2.transform->position, 0.5f);

    Texture EngineOVObjectIconGui("Assets/background.png");
    renderer.tex = EngineOVObjectIconGui;
    renderer2.tex = EngineOVObjectIconGui;

    updateFPSTimer.start();
    int frames = 0;
    int targetFPS = 200;
    long long targetFrameTimeNs = static_cast<long long>(1e9 / targetFPS);
    long long sleepThresholdNs = targetFrameTimeNs;

    Timer inputTimer;
    Shader classic_shader(vertexShaderSource, fragmentShaderSource);
    Shader unlit_shader(vertexShaderSource, unlitFrag);


    glm::vec2 mousePre = window.mouseAsWorldPosition(camera1);
    glm::vec2 mouseCur = window.mouseAsWorldPosition(camera1);

    OverDepth gizmos;
    //gizmos.setShader(unlit_shader);
    while (window.windowRunning())
    {
        fpsTimer.start();
        frames++;

        window.clear();
        camera1.updateMatrix(0.1f, 100.0f);
        classic_shader.Activate();
        
        Simplex simplex;
        if (GJK::isTouching(coll, coll2, simplex))
        {
            auto result =  EPA::getResolution(coll, coll2, simplex);
            std::cout << "depth: " << result.depth << '\n';
        }
        renderer.draw(window.getWindow(), classic_shader.ID, camera1, glm::vec3(0, 0, 1));
        renderer2.draw(window.getWindow(), classic_shader.ID, camera1, glm::vec3(0, 0, 1));
<<<<<<< HEAD
=======
        gizmos.line(unlit_shader.ID, camera1, obj.transform->position, obj2.transform->position, 5, glm::vec3(1, 0, 0));
>>>>>>> 72062ac4fcd1d4b315202b60ad428edb10b96377
        mousePre = mouseCur;
        mouseCur = window.mouseAsWorldPosition(camera1);
        
        if (renderer.checkMouseBoundry(mouseCur, window.width, window.height))
        {
            if (InputSystem::getHold(Inputs::MouseRight))
            {
                obj.transform->position += (mouseCur - mousePre);
                coll.position = obj.transform->position;
            }
        }
        window.update();

        // Limit FPS
        while (fpsTimer.getElapsedNanoSeconds() < sleepThresholdNs);
    }

    return 0; // Return 0 for successful execution
}
