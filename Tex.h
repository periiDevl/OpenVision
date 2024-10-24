#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H
#include "Shader.h"
#include<glad/glad.h>
#include<stb/stb_image.h>
#include<iostream>

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);
	Texture(const char* image);
	Texture(GLuint framebufferTexture, GLuint slot);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};
#endif