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
#include"PhysicsWorld.h"
#include "Object.h"
#include "Shader.h"
class TextureRenderer : public Component
{
public:
	using Component::Component;
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	Texture tex = Texture("");
	void init()
	{
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
		model = glm::translate(model, glm::vec3(gameObject.transform->position, 0) / glm::vec3(4));

		model = glm::rotate(model, Deg(gameObject.transform->rotation), axis);
		model = glm::scale(model, glm::vec3(gameObject.transform->scale, 1.0f));

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
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

	
private:
	
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