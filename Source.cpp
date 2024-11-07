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

#include "IMGUITheme.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// Function declarations
CSV vert;
CSF frag;
float far = 300.000f;
glm::vec3 lightPos = glm::vec3(0.5f, 0.8f, 0.5f);

float gamma = 1.6f;
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
    Shader unlitShader(vertexShaderSource, unlitFrag);
    Shader frameBufferShader(vert.Frame, frag.Frame);
    Shader frameBufferShader2D(FrameBufferVert, FrameBufferFrag);
    Shader shaderProgram(vert.Default, frag.Default);
    Shader shadowMapProgram(vert.ShadowMap, frag.NONE);
    Shader blurProgram(vert.Frame, frag.Blur);
    // Framebuffer settings
    frameBufferShader.Activate();
    glUniform1i(glGetUniformLocation(frameBufferShader.ID, "screenTexture"), 0);
    glUniform1i(glGetUniformLocation(frameBufferShader.ID, "bloomTexture"), 1);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "radius"), 1.0);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "softness"), 0.2f);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "minEdgeContrast"), 128);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "subPixelAliasing"), 8);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "maximumEdgeDetection"), 128);

    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "gamma"), gamma);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "exposure"), 1);

    glUniform2f(glGetUniformLocation(frameBufferShader.ID, "resolution"), window.width, window.height);

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

    blurProgram.Activate();
    glUniform1i(glGetUniformLocation(blurProgram.ID, "screenTexture"), 0);
    glUniform1f(glGetUniformLocation(blurProgram.ID, "spreadBlur"), 1.7f);

    // Framebuffer instances
    Framebuffer mainFramebuffer(window.width, window.height);
    Framebuffer mouseDetectionFramebuffer(window.width, window.height);
    //Shadow framebuffer

    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;

    // Create Frame Buffer Object
    unsigned int postProcessingFBO;
    glGenFramebuffers(1, &postProcessingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

    // Create Framebuffer Texture
    unsigned int postProcessingTexture;
    glGenTextures(1, &postProcessingTexture);
    glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window.width, window.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

    unsigned int bloomTexture;
    glGenTextures(1, &bloomTexture);
    glBindTexture(GL_TEXTURE_2D, bloomTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window.width, window.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture, 0);

    // Tell OpenGL we need to draw to both attachments
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    // Error checking framebuffer
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;

    // Create Ping Pong Framebuffers for repetitive blurring
    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffer[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.width, window.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);

        fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Ping-Pong Framebuffer error: " << fboStatus << std::endl;
    }


// Create shadow map framebuffer using the updated Framebuffer class
    bool renderShadows = true;
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    DefaultTheme();
    ImGuiStyle& style = ImGui::GetStyle();
    while (window.windowRunning()) {
        //orthgonalProjection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, far);
        //lightView = glm::lookAt(1300.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //flightProjection = orthgonalProjection * lightView;
       
        orthgonalProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.0f, far);
        lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightProjection = orthgonalProjection * lightView;
        
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
        if (renderShadows) {
            gird.Draw(shadowMapProgram, camera3D, 1, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
            gird1.Draw(shadowMapProgram, camera3D, 1, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 5.0f, 20.0f));
        }
        mainFramebuffer.bind();
        glViewport(0, 0, window.width, window.height);

        glEnable(GL_DEPTH_TEST);
        shaderProgram.Activate();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.getTexture());
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);
        window.clear();

        glEnable(GL_DEPTH_TEST);
        gird.Draw(shaderProgram, camera3D, 1, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        gird1.Draw(shaderProgram, camera3D, 1, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 5.0f, 20.0f));

        // Make it so the multisampling FBO is read while the post-processing FBO is drawn
        camera3D.Inputs(window.getWindow(), 1, 2);
        camera3D.Mouse(window.getWindow());


        if (InputSystem::getHold(Inputs::KeyF)) {
            mainFramebuffer.textureColorBuffer = shadowFramebuffer.getTexture();
        }
        // Render objects
        mainFramebuffer.bind();
        frameBufferShader2D.Activate();
        glm::vec2 mousePos = window.mouseAsWorldPosition(camera1);
        renderer.setShader(classicShader);
        renderer.draw(camera1);
        renderer.tex = Texture(shadowFramebuffer.getTexture(), 0);
        renderer2.setShader(classicShader);
        renderer2.draw(camera1);
        if (InputSystem::getDown(Inputs::MouseLeft) && !gizmos.isDragging()) {
            selectedObj = mouseDetect.ID_OVER_OBJECT(window, mouseDetectionFramebuffer, unlitShader, camera1, objects);
        }
        if (selectedObj > -1) {

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
        gizmos.line(glm::vec2(0, .05), glm::vec2(0, -.05), 4, glm::vec3(0));
        gizmos.line(glm::vec2(.05, 0), glm::vec2(-.05, 0), 4, glm::vec3(0));

        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFramebuffer.FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
        glBlitFramebuffer(0, 0, window.width, window.height, 0, 0, window.width, window.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        bool horizontal = true, first_iteration = true;
        int Blur_amount = 10;
        blurProgram.Activate();


        for (unsigned int i = 0; i < Blur_amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            glUniform1i(glGetUniformLocation(blurProgram.ID, "horizontal"), horizontal);

            if (first_iteration)
            {
                glBindTexture(GL_TEXTURE_2D, bloomTexture);
                first_iteration = false;
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
            }

            glBindVertexArray(mainFramebuffer.rectVAO);
            glDisable(GL_DEPTH_TEST);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            horizontal = !horizontal;
        }


        mainFramebuffer.DRAW_SCENE_AS_MAIN_FRAMEBUFFER(frameBufferShader, postProcessingTexture, pingpongBuffer, horizontal);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        //UI
        ImGui::Begin("Lighting");

        // Display the shadow texture on the left
        ImGui::BeginGroup();
        ImGui::Image((ImTextureID)shadowFramebuffer.getTexture(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndGroup();

        ImGui::SameLine(); // Move to the same row but keep in a new column

        // Begin the group of controls in a separate column to the right of the texture
        ImGui::BeginGroup();
        ImGui::Text("Render Shadows (disable FrameBuffer)");
        ImGui::SameLine();
        ImGui::Checkbox("", &renderShadows);
                   
        // Add sliders with proper alignment
        ImGui::SliderFloat("X", &lightPos.x, -2.5f, 2.5f);
        ImGui::SliderFloat("Y", &lightPos.y, 1, 4.5f);
        ImGui::SliderFloat("Z", &lightPos.z, -3.5f, 3.5f);
                                        
        ImGui::InputInt("ShadowMap X", &shadowMapWidth,0);
        ImGui::InputInt("ShadowMap Y", &shadowMapHeight,0);
        ImGui::EndGroup();              
        ImGui::Separator();

        ImGui::End();                   
                                        
        //UI                            
                                        
                                        
        ImGui::Render();                
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.update();                
                                        
                                        
        while (fpsTimer.getElapsedNanoSeconds() < 5000000);
    }                                   
                                        
                                        
    return 0;                           
}                                       
                                        