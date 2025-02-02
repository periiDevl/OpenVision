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
#include "EngineValues.h"
#include "PackagesSystem.h"
#include "OverDepth.h"
// Function declarations
CSV vert;
CSF frag;


using namespace physics2D;
float skyboxVertices[] =
{
    //   Coordinates
    -1.0f, -1.0f,  1.0f,//        7--------6
     1.0f, -1.0f,  1.0f,//       /|       /|
     1.0f, -1.0f, -1.0f,//      4--------5 |
    -1.0f, -1.0f, -1.0f,//      | |      | |
    -1.0f,  1.0f,  1.0f,//      | 3------|-2
     1.0f,  1.0f,  1.0f,//      |/       |/
     1.0f,  1.0f, -1.0f,//      0--------1
    -1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
    1, 2, 6,
    6, 5, 1,
    0, 4, 7,
    7, 3, 0,
    4, 5, 6,
    6, 7, 4,
    0, 3, 2,
    2, 1, 0,
    0, 1, 5,
    5, 4, 0,
    3, 7, 6,
    6, 2, 3
};

int main() 
{
    Values values;
    SteamAPI_Init();
    PackageSystem packageSystem;

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
    Shader skyboxShader(vert.Skybox, frag.Skybox);
    skyboxShader.activate();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

    frameBufferShader.activate();
    frameBufferShader.setInt("screenTexture", 0);
    frameBufferShader.setInt("bloomTexture", 1);

    frameBufferShader.setVec2("resolution", glm::vec2(window.v_width, window.v_height));

    shaderProgram.activate();
    
    shaderProgram.setFloat("worldRadius", 800.0f);


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
    Model gird1("models/mapone/scene.gltf");
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
    float GuiX = 0;
    float GuiY = 0;
    float GuiZ = 0;
    camera2D.zoom = .4;

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    std::string facesCubemap[6] =
    {
        "skybox/sky/right.jpg",
        "skybox/sky/left.jpg",
        "skybox/sky/top.jpg",
        "skybox/sky/bottom.jpg",
        "skybox/sky/front.jpg",
        "skybox/sky/back.jpg"
    };

    unsigned int cubemapTexture;
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else
        {

            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
            stbi_image_free(data);
        }
    }

    while (window.windowRunning()) 
    {
        SteamAPI_RunCallbacks();
        //std::cout << dynaLL.ReciveStringDLL() << std::endl;
        //dynaLL.SendStringToDll("Hello");
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
        camera3D.updateMatrix3D(60, 0.1f, values.camFar);
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
            gird.Draw(shadowMapProgram, camera3D, glm::vec3(GuiX / 2, GuiY/2, GuiZ/2), glm::vec3(0, 0, 0), glm::vec3(10.0f));

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
        shaderProgram.setVec3("lightPos", direcLight.getRawLightPosition());
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.getTexture());
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);
        window.clear();

        glEnable(GL_DEPTH_TEST);
        gird.Draw(shaderProgram, camera3D, glm::vec3(GuiX/2, GuiY/2, GuiZ/2), glm::vec3(0, 0, 0), glm::vec3(10.0f));

        gird.Draw(shaderProgram, camera3D, glm::vec3(11, 0,0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        
        gird.Draw(shaderProgram, camera3D, glm::vec3(0, 11, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        gird.Draw(shaderProgram, camera3D, glm::vec3(0, -11, 0), glm::vec3(0, 0, 0), glm::vec3(10.0f));
        grass.Draw(shaderProgram, camera3D, glm::vec3(0, -10, 0), glm::vec3(0, 0, 0), glm::vec3(5.0f));
        gird1.Draw(shaderProgram, camera3D, glm::vec3(0, -20, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 20.0f, 20.0f));


        camera3D.Inputs(window.getWindow(), 1, 2);
        camera3D.Mouse(window.getWindow());
        glDepthFunc(GL_LEQUAL);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::mat4(glm::mat3(glm::lookAt(camera3D.Position, camera3D.Position + camera3D.Orientation, camera3D.Up)));
        projection = glm::perspective(glm::radians(60.0f), (float)window.v_width / window.v_height, 0.1f, values.camFar);

        skyboxShader.activate();

        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
        // Blit to post-processing framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFramebuffer.FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFramebuffer.FBO);
        glBlitFramebuffer(0, 0, window.v_width, window.v_height, 0, 0, window.v_width, window.v_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        bool horizontal = true, first_iteration = true;
        blurProgram.activate();
        int Blur_amount = 12;
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
        if (InputSystem::getMousePosition().y > (window.height - window.v_height) && InputSystem::getMousePosition().x < window.v_width)
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


        gizmos.line(glm::vec3(GuiX, 0, 0), glm::vec3(GuiX + 10, 0, 0), 4, glm::vec3(1), camera3D, window.v_width, window.v_height, 60, 0.1f, 100.0f, camera2D, mousePos, window.getWindow(), GuiX, glm::vec3(1, 0, 0));
        gizmos.line(glm::vec3(0,GuiY, 0), glm::vec3(0,GuiY + 10, 0), 4, glm::vec3(1), camera3D, window.v_width, window.v_height, 60, 0.1f, 100.0f, camera2D, mousePos, window.getWindow(), GuiY, glm::vec3(0, 1, 0));
        gizmos.line(glm::vec3(0,0,GuiZ), glm::vec3(0, 0, GuiZ + 10), 4, glm::vec3(1), camera3D, window.v_width, window.v_height, 60, 0.1f, 100.0f, camera2D, mousePos,window.getWindow(), GuiZ, glm::vec3(0, 0,1));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        //UI
        ImGui::Begin("Scripts");
        ImGui::End();

        packageSystem.GUIOpenVision();
        ImGui::Separator();
        if (ImGui::Button("Create Item"))
        {
            packageSystem.createItem("Hellworld23", "Hello desc", "F:/Development/C++/OpenVision/OpenVision(Engine)/EngineAssets/mmsicon.png", "F:/Development/C++/OpenVision/OpenVision(Engine)/EngineAssets");
        }
        ImGui::End();

        ImGui::Begin("Objects");
        ImGui::End();

        ImGui::Begin("Lighting");
        ImGui::Text("OpenVision 3D graphics :");
        ImGui::Text("Default settings are fine, but in case of more control over the graphical");
        ImGui::Text("fidelity, there are several settings in this window to improve or change");
        ImGui::Text("the graphics. Make sure your hardware is capable of running such settings.");
        values.GeneralLightingValues(shaderProgram, direcLight);
        values.GammaAndExposureUI(frameBufferShader);
        values.antiAliasingUI(frameBufferShader);
        //values.VignetteUI(frameBufferShader);

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
        ImGui::Separator();
        values.ShadowUI(shaderProgram, direcLight);
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
                                        