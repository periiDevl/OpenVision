#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>


class Texture
{
public:
	GLuint ID;
	const char* type;
	const char* ImageFile = "Texture not loaded";
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);

	void texUnit(GLuint shader, const char* uniform);
	void Bind();
	void Unbind();
	void Delete();
};
#endif