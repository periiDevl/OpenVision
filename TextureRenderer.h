#ifndef TEXTURE_RENDERER_CLASS_H
#define TEXTURE_RENDERER_CLASS_H

#include "GameObject.h"
#include "Component.h"
#include "ResourceManager.h"
#include "glfw3.h"
#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Tex.h"
#include "Object.h"
#include "Shader.h"
class TextureRenderer : public Component
{
public:
	using Component::Component;
	
	TextureRenderer(GameObject& owner) 
		: Component(owner)
	{
		Vertex vert[] =
		{
			Vertex{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, -1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, -1.0f)},
			Vertex{glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
		};

		GLuint indi[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		vertices = std::vector <Vertex>(vert, vert + sizeof(vert) / sizeof(Vertex));
		indices = std::vector <GLuint>(indi, indi + sizeof(indi) / sizeof(GLuint));


		VAO.Bind();
		VBO VBO(vertices);
		EBO EBO(indices);
		VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
		VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
		VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
		//gameObject.transform->position
		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	}

	Texture tex = Texture("");

	void draw(GLFWwindow* window, GLuint shader, Camera& camera, glm::vec3 axis)
	{
		shader = shader;
		glUseProgram(shader);
		VAO.Bind();
		/*
		if (!TextureNearest) {
			tex.SetFilteringMode(GL_LINEAR, GL_LINEAR);
		}
		else {
			tex.SetFilteringMode(GL_NEAREST, GL_NEAREST);
		}
		*/
		tex.Bind();

		
		glUniform4f(glGetUniformLocation(shader, "tint"), tint.x, tint.y, tint.z, tint.w);

		glUniform1f(glGetUniformLocation(shader, "tileX"), TileX);
		glUniform1f(glGetUniformLocation(shader, "tileY"), TileY);
		if (cornerRadius != 0) {
			glUniform1f(glGetUniformLocation(shader, "cornerRadius"), cornerRadius);
		}
		else {
			glUniform1f(glGetUniformLocation(shader, "cornerRadius"), 0.01);

		}
		

		unsigned int numDiffuse = 0;
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(gameObject.transform->scale, 1.0f));
		model = glm::rotate(model, Deg(gameObject.transform->rotation), axis);
		model = glm::translate(model, glm::vec3(gameObject.transform->position, 0));

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

<<<<<<< HEAD
		glUniform3f(glGetUniformLocation(shader, "camPos"), camera.position.x, camera.position.y, camera.position.z);
=======
		glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

>>>>>>> 72062ac4fcd1d4b315202b60ad428edb10b96377
		camera.Matrix(shader, "camMatrix");
		//Used
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}


	void setTexture(std::string path)
	{
	}
	void roundCorners(int r)
	{

	}
	void pointToShader()
	{

	}
	void nearestTex(bool t)
	{

	}

	bool checkMouseBoundry(glm::vec2 mousePos, int wid, int hei)
	{
		// Update the gameObjectect's position to center it at the mouse position
		if (mousePos.x < gameObject.transform->position.x + gameObject.transform->scale.x * 0.5
			&& mousePos.x > gameObject.transform->position.x - gameObject.transform->scale.x * 0.5
			&& mousePos.y < gameObject.transform->position.y + gameObject.transform->scale.y * 0.5
			&& mousePos.y > gameObject.transform->position.y - gameObject.transform->scale.y * 0.5)
		{
			return true;
		}
		return false;

	}
	// Store the offset from the initial click

	// Call this function when the left mouse button is held down
	void snapToMouse(glm::vec2 mousePos, int wid, int hei)
	{
		// Calculate the offset only if it's the first call during this drag
		if (offset == glm::vec2(0.0f, 0.0f)) {
			// Store the offset based on the initial click
			offset = glm::vec2(mousePos.x, mousePos.y) - glm::vec2(gameObject.transform->position.x, gameObject.transform->position.y);
		}

		// Update the object's position to follow the mouse with the initial offset
		gameObject.transform->position.x = mousePos.x - offset.x;
		gameObject.transform->position.y = mousePos.y - offset.y;
	}

	// Call this function when the mouse button is released to reset the offset
	void releaseMouse()
	{
		offset = glm::vec2(0.0f, 0.0f); // Reset offset when the mouse button is released
	}


private:
	glm::vec2 offset = glm::vec2(0.0f, 0.0f);
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	float Deg(float radians)
	{

		return radians * 3.14159f / 180;
	}
	GLFWwindow* window;
	GLuint shader;

	glm::vec4 tint = glm::vec4(1);
	float cornerRadius = 0.001f;
	float TileX = 1.0f, TileY = 1.0f;
	std::vector <Texture> textures;
	VAO VAO;
};

#endif