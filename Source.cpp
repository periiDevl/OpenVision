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
#include "ShaderManager.h"
#include "EventManager.h"
// Function declarations
CSV vert;
CSF frag;
float far = 300.000f;
glm::vec3 lightPos = glm::vec3(0.5f, 0.8f, 0.5f);

float gamma = 1.6f;
float exposure = 1.0f;
int main() 
{
    EventManager::initialize();

    // Timers
    Timer fpsTimer, updateFPSTimer;
    updateFPSTimer.start();

    // Camera and window setup
    Window window = Window();
    Camera3D camera3D(1280, 800, glm::vec3(0.0f, 0.0f, 0.2f));
    Camera2D camera2D(glm::vec3(0.0f, 0.0f, 0.2f), 1280, 800);
    

    // Game objects

    std::vector<std::unique_ptr<GameObject>> objects;
    objects.push_back(std::make_unique<GameObject>());
    objects.push_back(std::make_unique<GameObject>());

    GameObject& obj = *objects[0];
    GameObject& obj2 = *objects[1];
    obj.transform->position = { 0, .5 };
    obj.transform->scale = { 1, 1 };
    obj2.transform->position = { 1.8, -.5 };
    obj2.transform->scale = { 1, 1 };


    // Renderer and texture setup
    TextureRenderer& renderer = *obj.addComponent<TextureRenderer>("Assets/background.png");
    TextureRenderer& renderer2 = *obj2.addComponent<TextureRenderer>("Assets/background.png");

    // Colliders
    CircleCollider coll(obj.transform->position, 0.5f);
    CircleCollider coll2(obj2.transform->position, 0.5f);

    // Shaders
    ShaderManager shaders;
    Shader& classicShader = shaders.addShader("classic", vertexShaderSource, fragmentShaderSource);
    Shader& unlitShader = shaders.addShader("unlit", vertexShaderSource, unlitFrag);
    Shader& frameBufferShader = shaders.addShader("frameBuffer", vert.Frame, frag.Frame);
    Shader& frameBufferShader2D = shaders.addShader("frameBuffer2D", FrameBufferVert, FrameBufferFrag);
    Shader& shaderProgram = shaders.addShader("shaderProgram", vert.Default, frag.Default);
    Shader& shadowMapProgram = shaders.addShader("shadowMapProgram", vert.ShadowMap, frag.NONE);
    Shader& blurProgram = shaders.addShader("blurProgram", vert.Frame, frag.Blur);

    // Framebuffer settings
    frameBufferShader.activate();
    glUniform1i(glGetUniformLocation(frameBufferShader.ID, "screenTexture"), 0);
    glUniform1i(glGetUniformLocation(frameBufferShader.ID, "bloomTexture"), 1);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "radius"), 1.0);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "softness"), 0.2f);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "minEdgeContrast"), 128);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "subPixelAliasing"), 8);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "maximumEdgeDetection"), 128);

    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "gamma"), gamma);
    glUniform1f(glGetUniformLocation(frameBufferShader.ID, "exposure"), exposure);

    glUniform2f(glGetUniformLocation(frameBufferShader.ID, "resolution"), window.width, window.height);

    shaderProgram.activate();

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

    blurProgram.activate();
    glUniform1i(glGetUniformLocation(blurProgram.ID, "screenTexture"), 0);
    glUniform1f(glGetUniformLocation(blurProgram.ID, "spreadBlur"), 1.7f);

    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;


    // Framebuffer instances


    // Create shadow map framebuffer using the updated Framebuffer class
    Framebuffer mainFramebuffer(window.width, window.height);
    Framebuffer mouseDetectionFramebuffer(window.width, window.height);
    PostProcessingFramebuffer postProcessingFramebuffer(window.width, window.height);
    //Shadow framebuffer
    bool renderShadows = true;
    int shadowMapWidth = 1000;
    int shadowMapHeight = 1000;
    Framebuffer shadowFramebuffer(shadowMapWidth, shadowMapHeight, true); // 'true' for depth-only

    glm::mat4 orthgonalProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.01f, far);
    glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightProjection = orthgonalProjection * lightView;


    shadowMapProgram.activate();
    glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

    glm::vec2 mousePos = camera2D.mouseAsWorldPosition();

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
    Model grass("models/grass/scene.gltf");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    DefaultTheme();
    ImGuiStyle& style = ImGui::GetStyle();
    while (window.windowRunning()) 
    {
        //orthgonalProjection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, far);
        //lightView = glm::lookAt(1300.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //flightProjection = orthgonalProjection * lightView;
       
        orthgonalProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.0f, far);
        lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightProjection = orthgonalProjection * lightView;
        
        //camera2D.updateMatrix(0.1f, far);
        camera3D.updateMatrix3D(60, 0.1f, far);
        mainFramebuffer.bind();
        window.clear();
        fpsTimer.start();
        mouseDetectionFramebuffer.resize(window.width, window.height);

        mainFramebuffer.resize(window.width, window.height);
        postProcessingFramebuffer.resize(window.width, window.height);
        glEnable(GL_DEPTH_TEST);
        glUniform2f(glGetUniformLocation(frameBufferShader.ID, "resolution"), window.width, window.height);
        shadowMapProgram.activate();
        glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
        shadowFramebuffer.bind();
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowMapProgram.activate();
        if (renderShadows) {
            gird.Draw(shadowMapProgram, camera3D,glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
            gird1.Draw(shadowMapProgram, camera3D,glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 5.0f, 20.0f));
        }
        mainFramebuffer.bind();
        glViewport(0, 0, window.width, window.height);

        glEnable(GL_DEPTH_TEST);
        shaderProgram.activate();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.getTexture());
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);
        window.clear();

        glEnable(GL_DEPTH_TEST);
        gird.Draw(shaderProgram, camera3D, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        gird.Draw(shaderProgram, camera3D, glm::vec3(0, 21, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        grass.Draw(shaderProgram, camera3D, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(5.0f));
        gird1.Draw(shaderProgram, camera3D, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 5.0f, 20.0f));


        camera3D.Inputs(window.getWindow(), 1, 2);
        camera3D.Mouse(window.getWindow());

        // Blit to post-processing framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFramebuffer.FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFramebuffer.FBO);
        glBlitFramebuffer(0, 0, window.width, window.height, 0, 0, window.width, window.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        bool horizontal = true, first_iteration = true;
        blurProgram.activate();
        int Blur_amount = 8;
        for (unsigned int i = 0; i < Blur_amount; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFramebuffer.pingpongFBO[horizontal]);
            glUniform1i(glGetUniformLocation(blurProgram.ID, "horizontal"), horizontal);

            glBindTexture(GL_TEXTURE_2D, first_iteration ? postProcessingFramebuffer.bloomTexture : postProcessingFramebuffer.pingpongBuffer[!horizontal]);
            first_iteration = false;

            glBindVertexArray(mainFramebuffer.rectVAO);
            glDisable(GL_DEPTH_TEST);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            horizontal = !horizontal;
        }


        mainFramebuffer.DRAW_SCENE_AS_MAIN_FRAMEBUFFER(frameBufferShader, postProcessingFramebuffer.texture, postProcessingFramebuffer.pingpongBuffer, horizontal);



        glm::vec2 mousePos = camera2D.mouseAsWorldPosition();
        renderer.setShader(classicShader);
        renderer.draw(camera2D);
        renderer.tex = Texture(shadowFramebuffer.getTexture(), 0);
        renderer2.setShader(classicShader);
        renderer2.draw(camera2D);
        
        if (InputSystem::getDown(Inputs::MouseLeft) && !gizmos.isDragging()) 
        {
            selectedObj = mouseDetect.ID_OVER_OBJECT(window, mouseDetectionFramebuffer, unlitShader, camera2D, objects);
        }
        if (selectedObj > -1) 
        {
            gizmos.scaleTextureGizmos(*objects[selectedObj].get(), mousePos, window);
            gizmos.worldGimzo(*objects[selectedObj].get(), mousePos, window);
            gizmos.Overlap(camera2D);
        }

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


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        //UI
        ImGui::Begin("Lighting");
        ImGui::Text("OpenVision 3D graphics :");
        ImGui::Text("Default settings are fine, but in case of more control over the graphical");
        ImGui::Text("fidelity, there are several settings in this window to improve or change");
        ImGui::Text("the graphics. Make sure your hardware is capable of running such settings.");

        ImGui::Separator();
        ImGui::SetNextItemWidth(80.0f);
        frameBufferShader.activate();
        ImGui::InputFloat("Gamma", &gamma);
        glUniform1f(glGetUniformLocation(frameBufferShader.ID, "gamma"), gamma);
        ImGui::SetNextItemWidth(80.0f);
        ImGui::InputFloat("Exposure", &exposure);
        glUniform1f(glGetUniformLocation(frameBufferShader.ID, "exposure"), exposure);

        ImGui::Separator();
        // Display the shadow texture on the left
        ImGui::BeginGroup();

        // Add sliders with proper alignment
        float lightX;
        float lightY;
        // Sliders to control light position

        ImGui::Image((ImTextureID)shadowFramebuffer.getTexture(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::SameLine();
        
        ImGui::VSliderFloat("Y", ImVec2(20, 200), &lightY, -1.57f * 100, 1.57f * 100);  // Rotation around X-axis (restricted range)
        ImGui::SameLine();
        ImGui::Image((ImTextureID)postProcessingFramebuffer.pingpongBuffer[!horizontal], ImVec2(70, 70), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::SetNextItemWidth(200);
        ImGui::SliderFloat("X", &lightX, -3.14f * 10, 3.14f * 10);  // Rotation around Y-axis
        // Rotation speed (this controls how fast the light moves without affecting the slider values)
        float rotationSpeed = 0.1f;  // You can adjust this for smoothness
        float adjustedLightX = lightX * rotationSpeed;  // Adjusted rotation for X (around the Y-axis)
        float adjustedLightY = lightY * rotationSpeed;  // Adjusted rotation for Y (around the X-axis)

        // Set the radius of light's circular path
        float radius = 5.0f;  // Distance from the origin (this can be adjusted)

        // Rotate the light around the Y-axis (X controls the angle here)
        lightPos.x = radius * cos(adjustedLightX);  // X position based on the angle
        lightPos.z = radius * sin(adjustedLightX);  // Z position based on the angle

        // Rotate the light around the X-axis (Y controls the vertical angle here)
        lightPos.y = adjustedLightY;  // Y position based on the height (vertical rotation)

        ImGui::EndGroup();

        ImGui::SameLine(); // Move to the same row but keep in a new column

        // Begin the group of controls in a separate column to the right of the texture
        ImGui::BeginGroup();
        ImGui::Text("Render Shadows (disable FrameBuffer)");
        ImGui::SameLine();
        ImGui::Checkbox("", &renderShadows);
                   
        ImGui::SetNextItemWidth(80.0f);

        
        // Optionally, you can adjust the sun's angle based on timeOfDay if you want to simulate the sun's position:
        // (For example, set the light rotation to correspond to the time of day)
        // For simplicity, the `lightPos.x` can already be used to control the sun's position via `adjustedLightX`.

        ImGui::SetNextItemWidth(60.0f);
        ImGui::InputInt("ShadowMap X", &shadowMapWidth,0);
        ImGui::SetNextItemWidth(60.0f);
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
                                        