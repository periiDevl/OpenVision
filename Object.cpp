#include "Object.h"

Object::Object(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
    : vertices(vertices), indices(indices), position(new glm::vec2(0.0f)), angle(0), scale(new glm::vec2(5.0f))
{
    name = "";
    selected = false;
    angle = new float(0);
   
    Body = new PhysicsBody(position, angle, scale, 1, 1, 0.5f, 0.5f, false, false);


    VAO.Bind();
    VBO VBO(vertices);
    EBO EBO(indices);
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Object::Draw(GLFWwindow* window, GLuint shader, Camera& camera, glm::vec3 axis, bool TextureNearest)
{
    shader = shader;
    glUseProgram(shader);
    VAO.Bind();
    if (!TextureNearest) {
        tex.SetFilteringMode(GL_LINEAR, GL_LINEAR);
    }
    else {
        tex.SetFilteringMode(GL_NEAREST, GL_NEAREST);
    }
    tex.Bind();

    glUniform4f(glGetUniformLocation(shader, "tint"), tint.x, tint.y, tint.z, tint.w);
    glUniform1f(glGetUniformLocation(shader, "tileX"), TileX);
    glUniform1f(glGetUniformLocation(shader, "tileY"), TileY);

    unsigned int numDiffuse = 0;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position->x, position->y, layer / 100.0));

    model = glm::rotate(model, Deg(*angle), axis);
    model = glm::scale(model, glm::vec3(*scale, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");
    //Used
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Object::Draw(GLFWwindow* window, GLuint shader, Camera& camera, glm::vec3 axis, float cameraX, float cameraY, bool TextureNearest)
{
    shader = shader;
    glUseProgram(shader);
    VAO.Bind();
    if (!TextureNearest) {
        tex.SetFilteringMode(GL_LINEAR, GL_LINEAR);
    }
    else {
        tex.SetFilteringMode(GL_NEAREST, GL_NEAREST);
    }
    tex.Bind();

    glUniform4f(glGetUniformLocation(shader, "tint"), tint.x, tint.y, tint.z, tint.w);

    glUniform1f(glGetUniformLocation(shader, "tileX"), TileX);
    glUniform1f(glGetUniformLocation(shader, "tileY"), TileY);

    unsigned int numDiffuse = 0;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position->x + cameraX, position->y + cameraY, layer / 100.0));

    model = glm::rotate(model, Deg(*angle), axis);
    model = glm::scale(model, glm::vec3(*scale, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");
    //Used
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Object::DrawTMP(GLFWwindow* window, GLuint shader, Camera& camera, glm::vec2 pos, glm::vec2 scale)
{
    shader = shader;
    glUseProgram(shader);
    VAO.Bind();
    tex.Bind();

    unsigned int numDiffuse = 0;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos.x, pos.y, layer / 100.0));

    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}


void Object::DrawCustomVertices(GLFWwindow* window, GLuint shader, Camera& camera, const std::vector<glm::vec3>& vertices)
{
    shader = shader;
    glUseProgram(shader);

    GLuint VAO;
    GLuint VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


