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

bool click;
void Object::Draw(GLFWwindow* window, GLuint shader, Camera& camera, double positionX, double positionY, float scalex, float scaley, float angle, glm::vec3 axis, float width, float height, glm::vec2 ratio)
{
    glUseProgram(shader);
    VAO.Bind();

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	float ndcMouseX = (float)mouseX / (float)width * 2.0f - 1.0f;
	float ndcMouseY = (float)mouseY / (float)height * 2.0f - 1.0f;
	ndcMouseX *= ratio.x * 4;
	ndcMouseY *= ratio.y * 4;

	if (-positionX - scalex / 3 < ndcMouseX && positionX + scalex / 3 > ndcMouseX && -positionY - scaley / 3 < ndcMouseY && positionY + scaley / 3 > ndcMouseY 
		&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		click = true;
		printf("gig");
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		click = false;
	}
	if (click == true)
	{
		positionX = positionX + ndcMouseX;
		positionY = positionY + ndcMouseY;

	}

    unsigned int numDiffuse = 0;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(positionX, -positionY, 0.0f));

    model = glm::rotate(model, angle, axis);
    model = glm::scale(model, glm::vec3(scalex, scaley, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}






