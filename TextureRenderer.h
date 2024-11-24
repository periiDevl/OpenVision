
#ifndef TEXTURE_RENDERER_CLASS_H
#define TEXTURE_RENDERER_CLASS_H

#include "GameObject.h"
#include "Component.h"
#include "ResourceManager.h"
#include "glfw3.h"
#include"VAO.h"
#include"EBO.h"
#include"Camera2D.h"
#include"Tex.h"
#include "Shader.h"

class TextureRenderer : public Component
{
public:
	using Component::Component;
	GLuint shader;
	
	TextureRenderer(GameObject& owner, const std::string& texturePath) : TextureRenderer(owner, texturePath.c_str())
	{ }

	TextureRenderer(GameObject& owner, const char* texturePath) : TextureRenderer(owner)
	{
		tex = Texture(texturePath);
	}

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
	void setShader(Shader shader)
	{
		this->shader = shader.ID;
	}
	void draw(Camera2D camera)
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

		model = glm::translate(model, glm::vec3(gameObject.transform->position, 0));
		model = glm::rotate(model, Deg(gameObject.transform->rotation), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(gameObject.transform->scale, 1.0f));


		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));


		glUniform3f(glGetUniformLocation(shader, "camPos"), camera.position.x, camera.position.y, camera.position.z);

		camera.Matrix(shader, "camMatrix");
		//Used
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	void draw(Camera2D camera, glm::vec3 position, glm::vec3 scale, Shader shade)
	{
		// Use the shader program
		glUseProgram(shade.ID);

		// Bind the Vertex Array Object
		VAO.Bind();

		// Bind the texture
		tex.Bind();

		// Set the tint color uniform
		glUniform4f(glGetUniformLocation(shade.ID, "tint"), tint.x, tint.y, tint.z, tint.w);

		// Set the tiling parameters
		glUniform1f(glGetUniformLocation(shade.ID, "tileX"), TileX);
		glUniform1f(glGetUniformLocation(shade.ID, "tileY"), TileY);

		// Set the corner radius uniform, using a default if not set
		glUniform1f(glGetUniformLocation(shade.ID, "cornerRadius"), (cornerRadius != 0) ? cornerRadius : 0.01f);

		// Set up the model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position); // Use input position
		model = glm::rotate(model, Deg(0), glm::vec3(0, 0, 1));
		model = glm::scale(model, scale); // Use input scale

		// Pass the model matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(shade.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Set the camera position uniform
		glUniform3f(glGetUniformLocation(shade.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

		// Set the camera matrix for the shader
		camera.Matrix(shade.ID, "camMatrix");

		// Draw the elements using the specified mode and count
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
	void snapToMouse(glm::vec2 mousePos)
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
	void snapToMouseX(glm::vec2 mousePos)
	{
		// Calculate the offset only if it's the first call during this drag
		if (offset == glm::vec2(0.0f, 0.0f)) {
			// Store the offset based on the initial click
			offset = glm::vec2(mousePos.x, mousePos.y) - glm::vec2(gameObject.transform->position.x, gameObject.transform->position.y);
		}

		// Update the object's position to follow the mouse with the initial offset
		gameObject.transform->position.x = mousePos.x - offset.x;
	}
	void snapToMouseY(glm::vec2 mousePos)
	{
		// Calculate the offset only if it's the first call during this drag
		if (offset == glm::vec2(0.0f, 0.0f)) {
			// Store the offset based on the initial click
			offset = glm::vec2(mousePos.x, mousePos.y) - glm::vec2(gameObject.transform->position.x, gameObject.transform->position.y);
		}

		// Update the object's position to follow the mouse with the initial offset
		gameObject.transform->position.y = mousePos.y - offset.y;
	}

	void snapScaleToMouse(glm::vec2 mousePos)
	{
		// Calculate the offset only if it's the first call during this drag
		if (offset == glm::vec2(0.0f, 0.0f)) {
			// Store the initial scale value to calculate the new scale based on mouse position
			offset = glm::vec2(gameObject.transform->scale.x, gameObject.transform->scale.y);
		}

		// Calculate the center of the game object's position
		glm::vec2 center = gameObject.transform->position; // Assuming position holds the center point

		// Calculate the new scale based on the distance from the object's position to the mouse position
		float newScaleX = std::abs(mousePos.x - center.x);
		float newScaleY = std::abs(mousePos.y - center.y);

		// Set the new scale based on mouse position
		gameObject.transform->scale.x = newScaleX * 2; // Scale based on the distance from the center to the mouse position
		gameObject.transform->scale.y = newScaleY * 2; // Scale separately for the Y axis

		// Print the position and scale for debugging
		std::cout << "Position: (" << gameObject.transform->position.x << ", "
			<< gameObject.transform->position.y << "), Scale: ("
			<< gameObject.transform->scale.x << ", " << gameObject.transform->scale.y << ")" << std::endl;
	}

	void snapScaleToMouseX(glm::vec2 mousePos) {
		// Calculate the offset only if it's the first call during this drag
		if (offset.x == 0.0f) {
			// Store the initial scale value to calculate the new scale based on mouse position
			offset.x = gameObject.transform->scale.x;
		}

		// Calculate the center of the game object's position
		glm::vec2 center = gameObject.transform->position; // Assuming position holds the center point

		// Calculate the new scale based on the distance from the object's position to the mouse position
		float newScaleX = std::abs(mousePos.x - center.x);

		// Set the new scale based on mouse position for X axis
		gameObject.transform->scale.x = newScaleX * 2; // Scale based on the distance from the center to the mouse position

		// Print the position and scale for debugging
		std::cout << "Position: (" << gameObject.transform->position.x << ", "
			<< gameObject.transform->position.y << "), Scale: ("
			<< gameObject.transform->scale.x << ", " << gameObject.transform->scale.y << ")" << std::endl;
	}

	void snapScaleToMouseY(glm::vec2 mousePos) {
		// Calculate the offset only if it's the first call during this drag
		if (offset.y == 0.0f) {
			// Store the initial scale value to calculate the new scale based on mouse position
			offset.y = gameObject.transform->scale.y;
		}

		// Calculate the center of the game object's position
		glm::vec2 center = gameObject.transform->position; // Assuming position holds the center point

		// Calculate the new scale based on the distance from the object's position to the mouse position
		float newScaleY = std::abs(mousePos.y - center.y);

		// Set the new scale based on mouse position for Y axis
		gameObject.transform->scale.y = newScaleY * 2; // Scale based on the distance from the center to the mouse position

		// Print the position and scale for debugging
		std::cout << "Position: (" << gameObject.transform->position.x << ", "
			<< gameObject.transform->position.y << "), Scale: ("
			<< gameObject.transform->scale.x << ", " << gameObject.transform->scale.y << ")" << std::endl;
	}

	// Call this function when the mouse button is released to reset the offset
	void releaseMouse()
	{
		offset = glm::vec2(0.0f, 0.0f); // Reset offset when the mouse button is released
	}
	glm::vec3 windowToObjectPosition(glm::vec2 windowPos, int windowWidth, int windowHeight, Camera2D& camera) {
		// Convert window position to Normalized Device Coordinates (NDC)
		float xNDC = (2.0f * windowPos.x) / windowWidth - 1.0f;
		float yNDC = 1.0f - (2.0f * windowPos.y) / windowHeight; // Invert Y-axis
		glm::vec4 ndcCoords = glm::vec4(xNDC, yNDC, 0.0f, 1.0f); // Z is 0 for 2D

		// Get the inverse of the camera's transformation matrix
		glm::mat4 invCamMatrix = glm::inverse(camera.cameraMatrix);

		// Transform the NDC coordinates to world space
		glm::vec4 worldCoords = invCamMatrix * ndcCoords;

		// Return the world position
		return glm::vec3(worldCoords.x, worldCoords.y, 0.0f);
	}
	glm::vec2 worldToWindowPosition(glm::vec3 worldPos, int windowWidth, int windowHeight, Camera2D& camera) {
		// Get the camera's transformation matrix

		// Transform the world position to clip space
		glm::vec4 clipSpace = camera.cameraMatrix * glm::vec4(worldPos, 1.0f);

		// Perform perspective divide to get Normalized Device Coordinates (NDC)
		glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;

		// Convert NDC to window coordinates
		float xWindow = (ndc.x + 1.0f) * 0.5f * windowWidth;
		float yWindow = (1.0f - ndc.y) * 0.5f * windowHeight; // Invert Y-axis for window coordinates

		return glm::vec2(xWindow, yWindow);
	}


private:
	float lerp(float start, float end, float t) {
		return start + t * (end - start);
	}
	glm::vec2 offset = glm::vec2(0.0f, 0.0f);
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	float Deg(float radians)
	{

		return radians * 3.14159f / 180;
	}
	GLFWwindow* window;

	glm::vec4 tint = glm::vec4(1);
	float cornerRadius = 0.001f;
	float TileX = 1.0f, TileY = 1.0f;
	std::vector <Texture> textures;
	VAO VAO;
};

#endif