#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>
#include<iostream>

class Texture
{
public:
	GLuint ID;
	const char* type;
	std::string ImageFile = "Texture not loaded";
	std::string FullImageFile = "Texture not loaded";
	GLuint unit;

	Texture(const char* image);

	void texUnit(GLuint shader, const char* uniform);
	void Bind();
	void Unbind();
	void Delete();
};
#endif