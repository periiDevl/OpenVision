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
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	VAO VAO;

	Object(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);

	void Draw(GLuint shader, Camera& camera, double mouseX, double mouseY, float scalex, float scaley, float angle, glm::vec3 axis);
};
#endif