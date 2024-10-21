#include <iostream>
#include <chrono>
#include <thread>
#include "TextureRenderer.h"
#include "Window.h"
#include "InputSystem.h"
#include "Timer.h"
#include "OV.h"
#include "Settings.h"
#include "Object.h"
#include "Shader.h"
#include "OVLIB.h"
#include "CircleCollider.h"
#include "GJK.h"
#include "EPA.h"
#include "OverDepth.h"
#include"FrameBuffer.h"
#include "MouseDetection.h"
// Function declarations

int main() {
    // Timers
    Timer fpsTimer, updateFPSTimer;
    updateFPSTimer.start();

    // Camera and window setup
    Camera camera1(glm::vec3(0.0f, 0.0f, 0.2f), 1280, 800);
    Window window(camera1);
    // Game objects

    std::vector<std::unique_ptr<GameObject>> objects;
    objects.push_back(std::make_unique<GameObject>());
    objects.push_back(std::make_unique<GameObject>());

    GameObject& obj = *objects[0];
    GameObject& obj2 = *objects[1];
    obj.transform->position = { -1, 0 };
    obj.transform->scale = { 1, 1 };
    obj2.transform->position = { 1, 0 };
    obj2.transform->scale = { 1, 1 };


    // Renderer and texture setup
    TextureRenderer& renderer = *obj.addComponent<TextureRenderer>();
    TextureRenderer& renderer2 = *obj2.addComponent<TextureRenderer>();
    Texture EngineOVObjectIconGui("Assets/background.png");
    renderer.tex = EngineOVObjectIconGui;
    renderer2.tex = EngineOVObjectIconGui;

    // Colliders
    CircleCollider coll(obj.transform->position, 0.5f);
    CircleCollider coll2(obj2.transform->position, 0.5f);

    // Shaders
    Shader classicShader(vertexShaderSource, fragmentShaderSource);
    Shader frameBufferShader(FrameBufferVert, FrameBufferFrag);
    Shader unlitShader(vertexShaderSource, unlitFrag);

    // Framebuffer settings
    frameBufferShader.Activate();
    glUniform1i(glGetUniformLocation(frameBufferShader.ID, "screenTexture"), 0);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "radius"), 1.0);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "softness"), 0.2f);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "minEdgeContrast"), 128);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "subPixelAliasing"), 8);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "maximumEdgeDetection"), 128);

    // Framebuffer instances
    Framebuffer mainFramebuffer(window.width, window.height);
    Framebuffer mouseDetectionFramebuffer(window.width, window.height);

    glm::vec2 mousePos = window.mouseAsWorldPosition(camera1);

    // Gizmos
    OverDepth gizmos;

    // Rectangle VAO setup
    mainFramebuffer.setUpRectangle();
    // Main loop

    MouseDetection mouseDetect;
    std::vector<TextureRenderer> detectableObjects = { renderer, renderer2 };
    int selectedObj = -1;
    while (window.windowRunning()) {
        fpsTimer.start();
        mouseDetectionFramebuffer.resize(window.width, window.height);
        mainFramebuffer.resize(window.width, window.height);

        mainFramebuffer.bind();
        window.clear();
        glEnable(GL_DEPTH_TEST);
        glUniform2f(glGetUniformLocation(frameBufferShader.ID, "resolution"), window.width, window.height);

        camera1.updateMatrix(0.1f, 100.0f);

        // GJK collision check
        Simplex simplex;
        if (GJK::isTouching(coll, coll2, simplex)) {
            auto result = EPA::getResolution(coll, coll2, simplex);
            std::cout << "Collision depth: " << result.depth << std::endl;
        }

        // Render objects
        glm::vec2 mousePos = window.mouseAsWorldPosition(camera1);
        renderer.setShader(classicShader);
        renderer.draw(camera1);
        renderer2.setShader(classicShader);
        renderer2.draw(camera1);
        if (InputHandler.getDown(Inputs::MouseLeft) && !gizmos.isDragging()) {
            selectedObj = mouseDetect.ID_OVER_OBJECT(window, mouseDetectionFramebuffer, unlitShader, camera1, objects);
        }
        if (selectedObj != -1) {
            
            gizmos.scaleTextureGizmos(*objects[selectedObj], mousePos, window);
            gizmos.worldGimzo(*objects[selectedObj], mousePos, window);
            gizmos.Overlap(camera1);
        }

        // Detect which object is under the mouse
        std::cout << selectedObj << std::endl;
        mainFramebuffer.DRAW_SCENE_AS_MAIN_FRAMEBUFFER(frameBufferShader);
        window.update();

        while (fpsTimer.getElapsedNanoSeconds() < 5000000);
    }


    return 0;
}
