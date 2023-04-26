#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Tex.h"

class Object
{
public:
	bool deleted = false;
	bool selected;
	Texture tex = Texture("epicphoto.jpg", "diffuse", 0);
	float PositionX = 0;
	float PositionY = 0;
	float outlineWidth = 0;
	glm::vec3 OutlineColor = glm::vec3(0,0,0);
	glm::vec2 calculatedPosition;
	float angle = 0;


	float ScaleY = 20;
	float ScaleX = 20;
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	VAO VAO;

	float Deg(float radians)
	{

		return radians * 3.14159 / 180;
	}

	Object(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
	void Draw(GLFWwindow* window, GLuint shader, Camera& camera,glm::vec3 axis, float width, float height, glm::vec2 ratio);
	void DrawTMP(GLFWwindow* window, GLuint shader, Camera& camera, glm::vec2 pos);

private :
	
};
#endif