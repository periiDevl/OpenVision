#include "Object.h"
Object::Object(std::vector <Vertex>& vertices, std::vector <GLuint>& indices)
{
	Object::vertices = vertices;
    Object::indices = indices;

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

void Object::Draw(GLuint shader, Camera& camera, double mouseX, double mouseY, float scalex, float scaley, float angle, glm::vec3 axis)
{
    glUseProgram(shader);
    VAO.Bind();

    unsigned int numDiffuse = 0;

    // Center the object at the mouse position
    float ndcMouseX = (float)mouseX / (float)(90 * 16) * 2.0f - 1.0f;
    float ndcMouseY = (float)mouseY / (float)(90 * 9) * 2.0f - 1.0f;
    ndcMouseX *= 16 * 4; // Increase speed
    ndcMouseY *= 9 * 4; // Increase speed

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(ndcMouseX, -ndcMouseY, 0.0f));

    model = glm::rotate(model, angle, axis);
    model = glm::scale(model, glm::vec3(scalex, scaley, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}






