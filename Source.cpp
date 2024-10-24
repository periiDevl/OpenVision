#include <iostream>
#include <chrono>
#include <thread>
#include "TextureRenderer.h"
#include "Window.h"
#include "InputSystem.h"
#include "Timer.h"
#include "OV.h"
#include "Settings.h"
#include "Shader.h"
#include "OVLIB.h"
#include "CircleCollider.h"
#include "GJK.h"
#include "GJK.h"
#include "EPA.h"
#include "OverDepth.h"
#include"FrameBuffer.h"
#include "MouseDetection.h"
#include "Model.h"
#include "CSV.h"
#include "CSF.h"
// Function declarations
CSV vert;
CSF frag;
float far = 300.000f;
glm::vec3 lightPos = glm::vec3(0.5f, 0.8f, 0.5f);
int main() {
    // Timers
    Timer fpsTimer, updateFPSTimer;
    updateFPSTimer.start();

    // Camera and window setup
    Camera camera3D(1280, 800, glm::vec3(0.0f, 0.0f, 0.2f));
    Camera2D camera1(glm::vec3(0.0f, 0.0f, 0.2f), 1280, 800);
    Window window(camera1);
    // Game objects

    std::vector<std::unique_ptr<GameObject>> objects;
    objects.push_back(std::make_unique<GameObject>());
    objects.push_back(std::make_unique<GameObject>());

    GameObject& obj = *objects[0];
    GameObject& obj2 = *objects[1];
    obj.transform->position = { 1.2, 0 };
    obj.transform->scale = { 1, 1 };
    obj2.transform->position = { 0, 0 };
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
    Shader shaderProgram(vert.Default, frag.Default);
    Shader shadowMapProgram(vert.ShadowMap, frag.NONE);
    // Framebuffer settings
    frameBufferShader.Activate();
    glUniform1i(glGetUniformLocation(frameBufferShader.ID, "screenTexture"), 0);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "radius"), 1.0);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "softness"), 0.2f);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "minEdgeContrast"), 128);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "subPixelAliasing"), 8);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "maximumEdgeDetection"), 128);
    shaderProgram.Activate();

    glUniform1f(glGetUniformLocation(shaderProgram.ID, "avgShadow"), 1.0f);
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), 1, 1, 1, 1);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), 0.1f);
    glUniform1f(glGetUniformLocation(shaderProgram.ID, "far"), far);

    glUniform1i(glGetUniformLocation(shaderProgram.ID, "BPL_Lighting"), true);


    glUniform1f(glGetUniformLocation(shaderProgram.ID, "worldRadius"), 800);

    glUniform1f(glGetUniformLocation(shaderProgram.ID, "bias1"), 0.005f); // maximum bias
    glUniform1f(glGetUniformLocation(shaderProgram.ID, "bias2"), 0.0005f); // minimum bias


    glUniform1i(glGetUniformLocation(shaderProgram.ID, "sampleRadius"), 1);
    // Framebuffer instances
    Framebuffer mainFramebuffer(window.width, window.height);
    Framebuffer mouseDetectionFramebuffer(window.width, window.height);
    //Shadow framebuffer


// Create shadow map framebuffer using the updated Framebuffer class
    int shadowMapWidth = 1000;
    int shadowMapHeight = 1000;
    Framebuffer shadowFramebuffer(shadowMapWidth, shadowMapHeight, true); // 'true' for depth-only

    glm::mat4 orthgonalProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.01f, far);
    glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightProjection = orthgonalProjection * lightView;


    shadowMapProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));



    glm::vec2 mousePos = window.mouseAsWorldPosition(camera1);

    // Gizmos
    OverDepth gizmos;

    // Rectangle VAO setup
    mainFramebuffer.setUpRectangle();
    // Main loop

    MouseDetection mouseDetect;
    std::vector<TextureRenderer> detectableObjects = { renderer, renderer2 };
    int selectedObj = -1;

    Model gird("models/cube/scene.gltf");
    Model gird1("models/cube/scene.gltf");
    while (window.windowRunning()) {
        //orthgonalProjection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, far);
        //lightView = glm::lookAt(1300.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //flightProjection = orthgonalProjection * lightView;

        camera1.updateMatrix(0.1f, far);
        camera3D.updateMatrix3D(60, 0.1f, far);
        mainFramebuffer.bind();
        window.clear();
        fpsTimer.start();
        mouseDetectionFramebuffer.resize(window.width, window.height);

        mainFramebuffer.resize(window.width, window.height);

        glEnable(GL_DEPTH_TEST);
        glUniform2f(glGetUniformLocation(frameBufferShader.ID, "resolution"), window.width, window.height);
        shadowMapProgram.Activate();
        glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

        shadowFramebuffer.bind();
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowMapProgram.Activate();

        gird.Draw(shadowMapProgram, camera3D, 1, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        gird1.Draw(shadowMapProgram, camera3D, 1, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 5.0f, 20.0f));

        mainFramebuffer.bind();
        glViewport(0, 0, window.width, window.height);

        glEnable(GL_DEPTH_TEST);
        shaderProgram.Activate();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.getTexture());
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);

        glEnable(GL_DEPTH_TEST);
        gird.Draw(shaderProgram, camera3D, 1, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        gird1.Draw(shaderProgram, camera3D, 1, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 5.0f, 20.0f));




        camera3D.Inputs(window.getWindow(), 1, 2);
        camera3D.Mouse(window.getWindow());


        if (InputSystem::getHold(Inputs::KeyF)) {
            mainFramebuffer.textureColorBuffer = shadowFramebuffer.getTexture();
        }


        // Render objects
        glm::vec2 mousePos = window.mouseAsWorldPosition(camera1);
        renderer.setShader(classicShader);
        renderer.draw(camera1);
        renderer.tex = Texture(shadowFramebuffer.getTexture(), 0);
        renderer2.setShader(classicShader);
        renderer2.draw(camera1);
        gird.scale = glm::vec3(0.5f);
        gird.rotation.x = 30;
        gird.rotation.y = 30;
        gird.rotation.z = 30;
        if (InputSystem::getDown(Inputs::MouseLeft) && !gizmos.isDragging()) {
            selectedObj = mouseDetect.ID_OVER_OBJECT(window, mouseDetectionFramebuffer, unlitShader, camera1, objects);
        }
        if (selectedObj != -1) {

            gizmos.scaleTextureGizmos(*objects[selectedObj], mousePos, window);
            gizmos.worldGimzo(*objects[selectedObj], mousePos, window);
            gizmos.Overlap(camera1);
        }

        // Detect which object is under the mouse
        std::cout << selectedObj << std::endl;


        // GJK collision check
        coll.position = obj.transform->position;
        coll2.position = obj2.transform->position;

        Simplex simplex;
        if (GJK::isTouching(coll, coll2, simplex))
        {
            auto result = EPA::getResolution(coll, coll2, simplex);

            if (InputSystem::getDown(Inputs::KeySpace))
            {
                obj.transform->position -= (float)result.depth * result.normal * 0.5f;
                obj2.transform->position += (float)result.depth * result.normal * 0.5f;

            }
        }
        gizmos.line(glm::vec2(0, .01), glm::vec2(0, -.01), 12, glm::vec3(0));
        gizmos.line(glm::vec2(.01, 0), glm::vec2(-.01, 0), 12, glm::vec3(0));
        mainFramebuffer.DRAW_SCENE_AS_MAIN_FRAMEBUFFER(frameBufferShader);
        window.update();

        while (fpsTimer.getElapsedNanoSeconds() < 5000000);
    }


    return 0;
}
