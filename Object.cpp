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
double beforeMouseX;
double beforeMouseY;
void Object::Draw(GLFWwindow* window, GLuint shader, Camera& camera, float angle, glm::vec3 axis, float width, float height, glm::vec2 ratio)
{
    glUseProgram(shader);
    VAO.Bind();

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	float newPosX;
	float newPosY;

	float ndcMouseX = (float)mouseX / (float)width * 2.0f - 1.0f;
	float ndcMouseY = (float)mouseY / (float)height * 2.0f - 1.0f;
	ndcMouseX *= ratio.x * 4;
	ndcMouseY *= ratio.y * 4;

	newPosX = PositionX + beforeMouseX;
	newPosY = PositionY + beforeMouseY;

	if (newPosX - ScaleX / 3 < ndcMouseX &&
		newPosX + ScaleX / 3 > ndcMouseX &&
		newPosY + ScaleY / 3 > ndcMouseY &&
		newPosY - ScaleY / 3 < ndcMouseY
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
		beforeMouseX = ndcMouseX;
		beforeMouseY = ndcMouseY;

	}

	
    unsigned int numDiffuse = 0;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(newPosX, -newPosY, 0.0f));

    model = glm::rotate(model, angle, axis);
    model = glm::scale(model, glm::vec3(ScaleX, ScaleY, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}






