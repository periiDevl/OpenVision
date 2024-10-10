#ifndef TEXTURE_RENDERER_CLASS_H
#define TEXTURE_RENDERER_CLASS_H

#include "GameObject.h"
#include "Component.h"
#include "ResourceManager.h"
#include "glfw3.h"

class TextureRenderer : public Component
{
public:
	using Component::Component;

	void init()
	{

	}

	void draw()
	{
		
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
	std::string texturePath;
	GLFWwindow* window;
	GLuint shader;
};

#endif