#ifndef LINE_RENDERER_CLASS_H
#define LINE_RENDERER_CLASS_H

#include "GameObject.h"
#include "Component.h"
#include "ResourceManager.h"
#include "glfw3.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Shader.h"
#include <vector>

class LineRenderer
{
public:
    LineRenderer()
    {
        // Define two vertices for a line
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

    void drawLine(GLFWwindow* window, GLuint shader, Camera& camera, glm::vec3 axis, glm::vec2 start, glm::vec2 end)
    {
        // Disable depth testing to draw over everything
        glDisable(GL_DEPTH_TEST);

        // Optionally, clear depth buffer to ensure no other object interferes
        glClear(GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        VAO.Bind();

        // Update line start and end positions
        vertices[0].position = glm::vec3(start, 0.0f);
        vertices[1].position = glm::vec3(end, 0.0f);

        // Update the VBO with new vertex data
        VBO->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
        VBO->Unbind();

        // Create model matrix (optional if you want to apply transformations)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, Deg(0.0f), axis);  // No rotation
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // No translation

        // Send uniforms to the shader
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Matrix(shader, "camMatrix");

        // Draw the line
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

        // Re-enable depth testing
        glEnable(GL_DEPTH_TEST);

        VAO.Unbind();
    }


private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    VAO VAO;
    VBO* VBO;  // Class member to store VBO object
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);

    float Deg(float radians)
    {
        return radians * 3.14159f / 180;
    }
};

#endif
