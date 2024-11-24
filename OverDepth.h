#ifndef OVERDEPTH_RENDERER_CLASS_H
#define OVERDEPTH_RENDERER_CLASS_H

#include "GameObject.h"
#include "Component.h"
#include "ResourceManager.h"
#include "glfw3.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera2D.h"
#include "Shader.h"
#include <vector>
#include "Camera.h"
class OverDepth
{
public:
    GLFWwindow* window;

    OverDepth()
        : unlit_shader(vertexShaderSource, unlitFrag), camera(glm::vec3(0.0f, 0.0f, 0.2f), 1280, 800), renderer(middle)

    {
        Vertex vert[] =
        {
            Vertex{glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
        };

        GLuint indi[] =
        {
            0, 1  // Indices for the line
        };

        vertices = std::vector<Vertex>(vert, vert + sizeof(vert) / sizeof(Vertex));
        indices = std::vector<GLuint>(indi, indi + sizeof(indi) / sizeof(GLuint));

        // Set up the VAO, VBO, and EBO for the line
        VAO.Bind();
        VBO = new class VBO(vertices); // Store the VBO in a class member
        EBO EBO(indices);

        // Link vertex attributes
        VAO.LinkAttrib(*VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
        VAO.LinkAttrib(*VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
        VAO.LinkAttrib(*VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
        VAO.LinkAttrib(*VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

        VAO.Unbind();
        VBO->Unbind();
        EBO.Unbind();
    }
    void Overlap(Camera2D cam) {
        camera = cam;
    }
 
    void line(glm::vec2 start, glm::vec2 end, float thickness, glm::vec3 color)
    {
        glLineWidth(thickness);
        // Disable depth testing to draw over everything
        glDisable(GL_DEPTH_TEST);

        // Optionally, clear depth buffer to ensure no other object interferes
        glClear(GL_DEPTH_BUFFER_BIT);

        glUseProgram(unlit_shader.ID);
        VAO.Bind();

        // Update line start and end positions
        vertices[0].position = glm::vec3(start, 0.0f);
        vertices[1].position = glm::vec3(end, 0.0f);

        // Update the VBO with new vertex data
        VBO->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        VBO->Unbind();

        // Create model matrix (optional if you want to apply transformations)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, Deg(0.0f), glm::vec3(0, 0, 1));  // No rotation
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // No translation

        // Send uniforms to the shader
        glUniformMatrix4fv(glGetUniformLocation(unlit_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(unlit_shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
        camera.Matrix(unlit_shader.ID, "camMatrix");
        glUniform3f(glGetUniformLocation(unlit_shader.ID, "color"), color.x, color.y, color.z);

        // Draw the line
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

        // Re-enable depth testing
        glEnable(GL_DEPTH_TEST);

        VAO.Unbind();
    }

    void line(glm::vec3 start, glm::vec3 end, float thickness, glm::vec3 color,
        Camera3D camera, int screenWidth, int screenHeight, float FOVdeg, float nearPlane, float frPlane, Camera2D cam2d, glm::vec2 mousePos,float& interX)
    {
        glLineWidth(thickness);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);

        glUseProgram(unlit_shader.ID);
        VAO.Bind();

        // Update line start and end positions
        vertices[0].position = start;
        vertices[1].position = end;

        // Update the VBO with new vertex data
        VBO->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        VBO->Unbind();

        // Get model, view, and projection matrices from the Camera3D object
        glm::mat4 model = glm::mat4(1.0f); // Assuming no additional transformations
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix(60.0f, 0.1f, 100.0f);

        // Combine matrices
        glm::mat4 mvp = projection * view * model;

        // Project 3D points to screen space
        glm::vec4 clipStart = mvp * glm::vec4(start, 1.0f);
        glm::vec4 clipEnd = mvp * glm::vec4(end, 1.0f);

        glm::vec2 screenStart = glm::vec2(
            (clipStart.x / clipStart.w * 0.5f + 0.5f) * screenWidth,
            (1.0f - (clipStart.y / clipStart.w * 0.5f + 0.5f)) * screenHeight // Flip Y-axis
        );


        glm::vec2 screenEnd = glm::vec2(
            (clipEnd.x / clipEnd.w * 0.5f + 0.5f) * screenWidth,
            (1.0f - (clipEnd.y / clipEnd.w * 0.5f + 0.5f)) * screenHeight
        );

        // Send uniforms to the shader
        glUniformMatrix4fv(glGetUniformLocation(unlit_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(unlit_shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Matrix(unlit_shader, "camMatrix");
        glUniform3f(glGetUniformLocation(unlit_shader.ID, "color"), color.x, color.y, color.z);

        // Draw the line
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

        glEnable(GL_DEPTH_TEST);
        VAO.Unbind();

        // Debug or use the calculated screen positions
        //std::cout << "Screen Start: (" << screenStart.x << ", " << screenStart.y << ")\n";
        //std::cout << "Screen End: (" << screenEnd.x << ", " << screenEnd.y << ")\n";
        line(cam2d.screenToWorld(screenEnd), cam2d.screenToWorld(glm::vec2(screenEnd.x, screenStart.y)), thickness, glm::vec3(1, 0, 0));
        line(cam2d.screenToWorld(glm::vec2(screenEnd.x, screenStart.y)), cam2d.screenToWorld(screenStart), thickness, glm::vec3(0, 0, 1));


        // Ensure the start and end points are correctly ordered
        float ex = screenEnd.x - screenStart.x;
        float ey = screenEnd.y - screenStart.y;

        


        float m = (ey / ex); 

        float b = screenStart.y - m * screenStart.x;


        /*
        std::cout << "Equation of the line: y = " <<- m << "x + " << b << "\n";
        */
        glm::vec2 mouseScreen = cam2d.worldToScreen(mousePos);


        float inverted_m = -1.0f / m;

        float perpendicular_b = mouseScreen.y - inverted_m * mouseScreen.x;

        float xIntersection = (perpendicular_b - b) / (m - inverted_m);

        float yIntersection = m * xIntersection + b;

        glm::vec2 intersectionScreen(xIntersection, yIntersection);
        glm::vec2 intersectionWorld = cam2d.screenToWorld(intersectionScreen);
        std::cout << "Intersection at: (" << intersectionWorld.x << ", " << intersectionWorld.y << ")\n";
        
        interX = cam2d.screenToWorld(intersectionScreen).x * 80;
        line(mousePos, cam2d.screenToWorld(intersectionScreen), thickness, glm::vec3(1, 0, 1));
        line(cam2d.screenToWorld(screenStart), cam2d.screenToWorld(screenEnd), thickness, glm::vec3(1, 1, 0));

        

    }



    bool checkMouseBoundary(glm::vec2 start, glm::vec2 end, float thickness__line, glm::vec2 mousePos)
    {
        // Convert the thickness to screen space using the window dimensions
        float thickness = thickness__line / 100;
        float halfThickness = thickness / 2.0f;

        // Vector from start to end of the line
        glm::vec2 lineVec = end - start;

        // Vector from start to the mouse position
        glm::vec2 mouseVec = mousePos - start;

        // Calculate the projection of mouseVec onto lineVec
        float lineLengthSquared = glm::dot(lineVec, lineVec);

        // Avoid division by zero (in case start and end are the same)
        if (lineLengthSquared == 0.0f) {
            return glm::distance(start, mousePos) <= halfThickness;
        }

        // Projection scalar (how far along the line the projection is)
        float t = glm::dot(mouseVec, lineVec) / lineLengthSquared;

        // Clamp t to be between 0 and 1 to get the closest point on the line segment
        t = glm::clamp(t, 0.0f, 1.0f);

        // Find the closest point on the line segment
        glm::vec2 closestPoint = start + t * lineVec;

        // Distance from the mouse position to the closest point on the line
        float distance = glm::distance(mousePos, closestPoint);

        // Check if the distance is within the line's thickness
        return distance <= halfThickness;
    }

    void scaleTextureGizmos(GameObject& object, glm::vec2 mousePos, Window& window) {
        glm::vec2 pivotLeftBottom = glm::vec2(object.transform->position.x - object.transform->scale.x / 2, object.transform->position.y - object.transform->scale.y / 2);
        glm::vec2 pivotLeftUp = glm::vec2(object.transform->position.x - object.transform->scale.x / 2, object.transform->position.y + object.transform->scale.y / 2);
        glm::vec2 pivotRightUp = glm::vec2(object.transform->position.x + object.transform->scale.x / 2, object.transform->position.y + object.transform->scale.y / 2);
        glm::vec2 pivotRightBottom = glm::vec2(object.transform->position.x + object.transform->scale.x / 2, object.transform->position.y - object.transform->scale.y / 2);
        //glm::vec2 pivotLeftUp(object.transform->position.x - object.transform->scale.x / 2, object.transform->position.y + object.transform->scale.y / 2);
        float blackThickness = 7.0f;
        float whiteThickness = 3.0f;
        line(pivotLeftBottom, pivotLeftUp, blackThickness, glm::vec3(0, 0, 0));
        line(pivotLeftBottom, pivotLeftUp, whiteThickness, glm::vec3(1));



        line(pivotLeftUp, pivotRightUp, blackThickness, glm::vec3(0, 0, 0));
        line(pivotLeftUp, pivotRightUp, whiteThickness, glm::vec3(1));

        line(pivotRightUp, pivotRightBottom, blackThickness, glm::vec3(0, 0, 0));
        line(pivotRightUp, pivotRightBottom, whiteThickness, glm::vec3(1));

        line(pivotRightBottom, pivotLeftBottom, blackThickness, glm::vec3(0, 0, 0));
        line(pivotRightBottom, pivotLeftBottom, whiteThickness, glm::vec3(1));
        if (InputSystem::getUp(Inputs::MouseLeft))
        {
            Dragging = "";
            object.getComponent<TextureRenderer>().releaseMouse();
            window.setCursor(Window::Cursors::ArrowCursor);


        }
        if (Dragging != "MD" && Dragging != "ML" && Dragging != "MU") {
            
            if (checkMouseBoundary(pivotLeftBottom, pivotLeftUp, 5, mousePos) || checkMouseBoundary(pivotRightUp, pivotRightBottom, 5, mousePos) || Dragging == "SX") {
                window.setCursor(Window::Cursors::HResizeCursor);
                if (InputSystem::getHold(Inputs::MouseLeft) || Dragging == "SX")
                {
                    object.getComponent<TextureRenderer>().snapScaleToMouseX(mousePos);
                    Dragging = "SX";
                }
            }
            else if (checkMouseBoundary(pivotLeftUp, pivotRightUp, 5, mousePos) || checkMouseBoundary(pivotRightBottom, pivotLeftBottom, 5, mousePos) || Dragging == "SY")
            {
                window.setCursor(Window::Cursors::VResizeCursor);
                if (InputSystem::getHold(Inputs::MouseLeft) || Dragging == "SY")
                {
                    object.getComponent<TextureRenderer>().snapScaleToMouseY(mousePos);
                    Dragging = "SY";
                }
            }
            else { window.setCursor(Window::Cursors::ArrowCursor); }
        }
        

    }
    void worldGimzo(GameObject& object, glm::vec2 mousePos, Window& window)
    {

        line(object.transform->position, glm::vec2(object.transform->position.x + 0.3, object.transform->position.y), 8, glm::vec3(1, 0, 0));
        line(object.transform->position, glm::vec2(object.transform->position.x, object.transform->position.y + 0.3), 8, glm::vec3(0, 1, 0));
        glUniform3f(glGetUniformLocation(unlit_shader.ID, "color"), 0.4, 0.4, 0.4);
        renderer.draw(camera, glm::vec3(object.transform->position, 0), glm::vec3(0.1, 0.1, 0), unlit_shader);
        middle.transform->position = object.transform->position;
        middle.transform->scale = glm::vec2(0.1f, 0.1f);
        if (InputSystem::getUp(Inputs::MouseLeft))
        {
            Dragging = "";
            object.getComponent<TextureRenderer>().releaseMouse();
            window.setCursor(Window::Cursors::ArrowCursor);
       
       
        }
        if (Dragging != "SX" && Dragging != "SY") {
            if (renderer.checkMouseBoundry(mousePos, window.width, window.height) || Dragging == "MD")
            {
                if (InputSystem::getHold(Inputs::MouseLeft) || Dragging == "MD")
                {
                    object.getComponent<TextureRenderer>().snapToMouse(mousePos);
                    Dragging = "MD";
                }
            }
            else if (checkMouseBoundary(object.transform->position, glm::vec2(object.transform->position.x + 0.3, object.transform->position.y), 10, mousePos) || Dragging == "ML")
            {
                if (InputSystem::getHold(Inputs::MouseLeft) || Dragging == "ML")
                {
                    object.getComponent<TextureRenderer>().snapToMouseX(mousePos);
                    Dragging = "ML";
                }
            }
            else if (checkMouseBoundary(object.transform->position, glm::vec2(object.transform->position.x, object.transform->position.y + 0.3), 10, mousePos) || Dragging == "MU")
            {
                if (InputSystem::getHold(Inputs::MouseLeft))
                {
                    object.getComponent<TextureRenderer>().snapToMouseY(mousePos);
                    Dragging = "MU";
                }
            }
       
        }
    }
    bool isDragging() {
        return Dragging != "";
    }
private:
    GameObject middle;
    TextureRenderer renderer;
    Camera2D camera;
    Shader unlit_shader;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    VAO VAO;
    VBO* VBO;  // Class member to store VBO object
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);

    float Deg(float radians)
    {
        return radians * 3.14159f / 180;
    }
    std::string Dragging = "";
};

#endif
