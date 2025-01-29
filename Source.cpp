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
#include "PhysicsWorld.h"
#include "DirectionalLight.h"
#include "IMGUITheme.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ShaderManager.h"
#include "EventManager.h"
#include "BoxCollider.h"
#include "DLL.h"
#include "steam_api.h"
// Function declarations
CSV vert;
CSF frag;
float farFOV = 300.000f;

using namespace physics2D;

float gamma = 1.6f;
float exposure = 1.0f;

bool CreateWorkshopItem(const char* title, const char* description, const char* previewImagePath)
{
    if (!SteamAPI_Init()) {
        return false;  // Steam failed to initialize
    }

    // Create new Workshop item
    SteamAPICall_t createCall = SteamUGC()->CreateItem(2472350, k_EWorkshopFileTypeCommunity);

    if (createCall == k_UGCUpdateHandleInvalid)
    {
        return false;  // Handle error - invalid handle
    }

    // Get the update handle
    UGCUpdateHandle_t updateHandle = SteamUGC()->StartItemUpdate(2472350, createCall);

    // Set the basic properties
    bool success = true;
    success &= SteamUGC()->SetItemTitle(updateHandle, title);
    success &= SteamUGC()->SetItemDescription(updateHandle, description);
    success &= SteamUGC()->SetItemPreview(updateHandle, previewImagePath);
    success &= SteamUGC()->SetItemVisibility(updateHandle, k_ERemoteStoragePublishedFileVisibilityPublic);

    if (!success)
    {
        return false;  // Failed to set properties
    }

    // Submit the item
    SteamAPICall_t submitCall = SteamUGC()->SubmitItemUpdate(updateHandle, "Initial version");

    // You might want to handle the callback for submission results here

    return true;
}
int main() 
{
    CreateWorkshopItem("Hello world", "Hello desc", "F:/Development/C++/OpenVision/OpenVision(Engine)/EngineAssets/OvTrashIcon.png");
    
    DirectionalLight direcLight;

    DLL dynaLL;
    dynaLL.loadDLL("DynaLL/x64/Debug/DynaLL.dll");
    EventManager::initialize();

    // Timers
    Timer fpsTimer, updateFPSTimer;
    updateFPSTimer.start();

    // Camera and window setup
    Window window = Window();
    window.setVieportSize(window.width / 1.5f, window.height / 1.5f);
    Camera3D camera3D(window.width, window.height, glm::vec3(0.0f, 0.0f, 0.2f));
    Camera2D camera2D(glm::vec3(0.0f, 0.0f, 0.2f), window.width, window.height);
    camera2D.setVieportSize(window.width / 1.5f, window.height / 1.5f);

    // Game objects

    std::vector<std::unique_ptr<GameObject>> objects;
    objects.push_back(std::make_unique<GameObject>());
    objects.push_back(std::make_unique<GameObject>());
    objects.push_back(std::make_unique<GameObject>());

    GameObject& obj = *objects[0];
    GameObject& obj2 = *objects[1];
    GameObject& obj3 = *objects[2];
    obj.transform->position = { 0, 2 };
    obj.transform->scale = { 1, 1 };
    obj2.transform->position = { 0, -1 };
    obj2.transform->scale = { 3, 1 };
    obj3.transform->position = { 0, .5};
    obj3.transform->scale = { 1, 1 };


    // Renderer and texture setup
    TextureRenderer& renderer = *obj.addComponent<TextureRenderer>("Assets/background.png");
    TextureRenderer& renderer2 = *obj2.addComponent<TextureRenderer>("Assets/background.png");
    TextureRenderer& renderer3 = *obj3.addComponent<TextureRenderer>("Assets/background.png");

    // Colliders
    BoxCollider coll(obj.transform->position, 0, { .5, .5 });// {1, 1});
    BoxCollider coll2(obj2.transform->position, 0, {1.5, .5});
    BoxCollider coll3(obj3.transform->position, 0, { .5, .5 }); 

    PhysicsWorld world({0, -10});
    world.addBody(coll, 1);
    world.addBody(coll2, 1, true);
    world.addBody(coll3, 1);

    

    // Shaders
    ShaderManager shaders;
    Shader& classicShader = shaders.addShader("classic", vertexShaderSource, fragmentShaderSource);
    Shader& unlitShader = shaders.addShader("unlit", vertexShaderSource, unlitFrag);
    Shader& frameBufferShader = shaders.addShader("frameBuffer", vert.Frame, frag.Frame);
    Shader& frameBufferShader2D = shaders.addShader("frameBuffer2D", FrameBufferVert, FrameBufferFrag);
    Shader& shaderProgram = shaders.addShader("shaderProgram", vert.Default, frag.Default);
    Shader& shadowMapProgram = shaders.addShader("shadowMapProgram", vert.ShadowMap, frag.NONE);
    Shader& blurProgram = shaders.addShader("blurProgram", vert.Frame, frag.Blur);

    frameBufferShader.activate();
    frameBufferShader.setInt("screenTexture", 0);
    frameBufferShader.setInt("bloomTexture", 1);
    frameBufferShader.setFloat("radius", 1.0f);
    frameBufferShader.setFloat("softness", 0.2f);
    frameBufferShader.setFloat("minEdgeContrast", 128.0f);
    frameBufferShader.setFloat("subPixelAliasing", 8.0f);
    frameBufferShader.setFloat("maximumEdgeDetection", 128.0f);

    frameBufferShader.setFloat("gamma", gamma);
    frameBufferShader.setFloat("exposure", exposure);
    frameBufferShader.setVec2("resolution", glm::vec2(window.v_width, window.v_height));

    shaderProgram.activate();
    shaderProgram.setFloat("avgShadow", 1.0f);
    shaderProgram.setVec4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    shaderProgram.setVec3("lightPos", direcLight.getRawLightPosition());
    shaderProgram.setFloat("near", 0.1f);
    shaderProgram.setFloat("far", farFOV);
    shaderProgram.setBool("BPL_Lighting", true);

    shaderProgram.setFloat("worldRadius", 800.0f);

    shaderProgram.setFloat("bias1", 0.005f);
    shaderProgram.setFloat("bias2", 0.0005f);

    shaderProgram.setInt("sampleRadius", 1);

    blurProgram.activate();
    blurProgram.setInt("screenTexture", 0);
    blurProgram.setFloat("spreadBlur", 1.7f);


    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;


    // Framebuffer instances


    // Create shadow map framebuffer using the updated Framebuffer class
    Framebuffer mainFramebuffer(window.v_width, window.v_height);
    Framebuffer mouseDetectionFramebuffer(window.v_width, window.v_height);
    PostProcessingFramebuffer postProcessingFramebuffer(window.v_width, window.v_height);
    Framebuffer shadowFramebuffer(direcLight.shadowMapWidth, direcLight.shadowMapHeight, true); // 'true' for depth-only
    direcLight.shaderSetup(shaderProgram);




    // Gizmos
    OverDepth gizmos;

    // Rectangle VAO setup
    mainFramebuffer.setUpRectangle();
    // Main loop

    MouseDetection mouseDetect;
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
    std::vector<Model> models;
    models.push_back(gird);
    models.push_back(grass);
    float interX = 0;
    camera2D.zoom = .4;
    while (window.windowRunning()) 
    {
        std::cout << dynaLL.ReciveStringDLL() << std::endl;
        dynaLL.SendStringToDll("Hello");
        glm::vec2 mousePos = camera2D.mouseAsWorldPosition(glm::vec2(window.v_width, window.v_height));

        coll.m_position = obj.transform->position;
        coll2.m_position = obj2.transform->position;
        coll3.m_position = obj3.transform->position;

        //std::cout << glm::to_string(coll2.m_scale) << std::endl;
         
        world.fixedUpdate(0.003); // will change to fixed delta time

        obj.transform->position = coll.m_position;
        obj2.transform->position = coll2.m_position;
        obj3.transform->position = coll3.m_position;

        if (InputSystem::getDown(Inputs::KeyO))
        {
            world.jump();
        }
        //orthgonalProjection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, far);
        //lightView = glm::lookAt(1300.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //flightProjection = orthgonalProjection * lightView;
        
        direcLight.calculateShadowsProj(camera3D);

        //camera2D.updateMatrix(0.1f, far);
        camera3D.updateMatrix3D(60, 0.1f, farFOV);
        mainFramebuffer.bind();
        window.clear();
        fpsTimer.start();
        mouseDetectionFramebuffer.resize(window.v_width, window.v_height);
        mainFramebuffer.resize(window.v_width, window.v_height);
        postProcessingFramebuffer.resize(window.v_width, window.v_height);
        glEnable(GL_DEPTH_TEST);
        glUniform2f(glGetUniformLocation(frameBufferShader.ID, "resolution"), window.v_width, window.v_height);
        shadowMapProgram.activate();
        glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(direcLight.getLightProjection()));
        shadowFramebuffer.bind();
        glViewport(0, 0, direcLight.shadowMapWidth, direcLight.shadowMapHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowMapProgram.activate();
        if (direcLight.renderShadows) {
            gird.Draw(shadowMapProgram, camera3D, glm::vec3(interX / 2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));

            gird.Draw(shadowMapProgram, camera3D, glm::vec3(11, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));

            gird.Draw(shadowMapProgram, camera3D, glm::vec3(0, 11, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
            gird.Draw(shadowMapProgram, camera3D, glm::vec3(0, -11, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
            grass.Draw(shadowMapProgram, camera3D, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(5.0f));
        }
        mainFramebuffer.bind();
        glViewport(0, 0, window.v_width, window.v_height);

        glEnable(GL_DEPTH_TEST);
        shaderProgram.activate();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(direcLight.getLightProjection()));
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"),direcLight.getRawLightPosition().x, direcLight.getRawLightPosition().y, direcLight.getRawLightPosition().z);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.getTexture());
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);
        window.clear();

        glEnable(GL_DEPTH_TEST);
        gird.Draw(shaderProgram, camera3D, glm::vec3(interX / 2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));

        gird.Draw(shaderProgram, camera3D, glm::vec3(11, 0,0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        
        gird.Draw(shaderProgram, camera3D, glm::vec3(0, 11, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        gird.Draw(shaderProgram, camera3D, glm::vec3(0, -11, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        grass.Draw(shaderProgram, camera3D, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(5.0f));
        //gird1.Draw(shaderProgram, camera3D, glm::vec3(0, -5, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 5.0f, 20.0f));


        camera3D.Inputs(window.getWindow(), 1, 2);
        camera3D.Mouse(window.getWindow());

        // Blit to post-processing framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFramebuffer.FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFramebuffer.FBO);
        glBlitFramebuffer(0, 0, window.v_width, window.v_height, 0, 0, window.v_width, window.v_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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



        renderer.setShader(classicShader);
        renderer.draw(camera2D);
        renderer.tex = Texture(shadowFramebuffer.getTexture(), 0);
        renderer2.setShader(classicShader);
        renderer2.draw(camera2D);
        renderer3.setShader(classicShader);
        renderer3.draw(camera2D);
        if (mousePos.y > 0 || InputSystem::getMousePosition().x < window.v_width)
        {
            if (InputSystem::getDown(Inputs::MouseLeft) && !gizmos.isDragging())
            {

                selectedObj = mouseDetect.ID_OVER_OBJECT(window, mouseDetectionFramebuffer, unlitShader, camera2D, objects);
            }
            if (selectedObj != -1)
            {
                gizmos.Overlap(camera2D);
                gizmos.worldGimzo(*objects[selectedObj].get(), mousePos, window);
                gizmos.scaleTextureGizmos(*objects[selectedObj].get(), mousePos, window);
            }
        }
        gizmos.line(glm::vec2(0, .05), glm::vec2(0, -.05), 4, glm::vec3(0));
        gizmos.line(glm::vec2(.05, 0), glm::vec2(-.05, 0), 4, glm::vec3(0));


        gizmos.line(glm::vec3(interX, 0, 0), glm::vec3(interX + 10, 0, 0), 4, glm::vec3(1), camera3D, window.v_width, window.v_height, 60, 0.1f, 100.0f, camera2D, mousePos,window.getWindow(), interX);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        //UI
        ImGui::Begin("Scripts");
        ImGui::End();
        ImGui::Begin("Packages");
        ImGui::Text("Ctrl-p to search open");
        ImGui::End();
        ImGui::Begin("Objects");
        ImGui::End();

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
        ImGui::SetNextItemWidth(80.0f);
        ImGui::InputFloat("i:", &direcLight.camShadowDistance);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(300.0f);
        ImGui::SliderFloat("Camera Shadow Draw Distance", &direcLight.camShadowDistance, 300, 5000);
        ImGui::Separator();
        // Display the shadow texture on the left
        ImGui::BeginGroup();


        ImGui::Image((ImTextureID)shadowFramebuffer.getTexture(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::SameLine();
        
        ImGui::VSliderFloat("Y", ImVec2(20, 200), &direcLight.Y, -1.57f * 100, 1.57f * 100);  // Rotation around X-axis (restricted range)
        ImGui::SameLine();
        ImGui::Image((ImTextureID)postProcessingFramebuffer.pingpongBuffer[!horizontal], ImVec2(70, 70), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::SetNextItemWidth(200);
        ImGui::SliderFloat("X", &direcLight.X, -3.14f * 10, 3.14f * 10);  // Rotation around Y-axis
        direcLight.calculateLightRawToCoords();
        ImGui::EndGroup();

        ImGui::SameLine(); // Move to the same row but keep in a new column

        // Begin the group of controls in a separate column to the right of the texture
        ImGui::BeginGroup();
        ImGui::Text("Render Shadows (disable FrameBuffer)");
        ImGui::SameLine();
        ImGui::Checkbox("", &direcLight.renderShadows);
                   
        ImGui::SetNextItemWidth(80.0f);


        // Optionally, you can adjust the sun's angle based on timeOfDay if you want to simulate the sun's position:
        // (For example, set the light rotation to correspond to the time of day)
        // For simplicity, the `lightPos.x` can already be used to control the sun's position via `adjustedLightX`.

        ImGui::SetNextItemWidth(60.0f);
        ImGui::InputInt("ShadowMap X", &direcLight.shadowMapWidth,0);
        ImGui::SetNextItemWidth(60.0f);
        ImGui::InputInt("ShadowMap Y", &direcLight.shadowMapHeight,0);
        ImGui::EndGroup();              
        ImGui::Separator();

        ImGui::End();                   
                                        
        //UI                            
                                        
                                        
        ImGui::Render();                
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.update();                
                                        
                                        
        while (fpsTimer.getElapsedNanoSeconds() < 5000000);
    }                                   
                                        
    dynaLL.clean();
    return 0;                           
}                                       
                                        