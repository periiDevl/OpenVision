#include<iostream>
#include<chrono>
#include "TextureRenderer.h"
#include "OvWindow.h"
#include "InputSystem.h"
#include "Timer.h"
#include <thread>


int main() 
{

    Timer fpsTimer;
    Timer updateFPSTimer;

    OvWindow window;

    GameObject obj;
    TextureRenderer& renderer = *obj.addComponent<TextureRenderer>();

    updateFPSTimer.start();

    int frames = 0;
    int targetFPS = 20000;
    long long targetFrameTimeNs = static_cast<long long>(1e9 / targetFPS);
    long long sleepThresholdNs = targetFrameTimeNs;

    Timer inputTimer;

    while (window.windowRunning()) 
    {
        fpsTimer.start();
        frames++;

        renderer.draw();
        window.update();


        if (updateFPSTimer.getElapsedSeconds() > 1) 
        {
            // Convert to seconds
            window.setTitle("FPS: ~" + std::to_string(frames));
            updateFPSTimer.start();
            frames = 0;
        }


        if (InputSystem::getDown(Inputs::KeyD))
        {
            int axis = InputSystem::getAxis(Inputs::KeyLeft, Inputs::KeyRight);
            std::cout << "value: " << axis << "\n";
        }

        if (InputSystem::getDown(Inputs::KeyT))
        {
            glm::vec2 value = InputSystem::getMousePosition();
        
            std::cout << value.x << " " << value.y << '\n';
        }

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
        
        
        // limit fps
        while (fpsTimer.getElapsedNanoSeconds() < sleepThresholdNs);
    }
}