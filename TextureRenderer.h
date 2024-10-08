#ifndef TEXTURE_RENDERER_CLASS_H
#define TEXTURE_RENDERER_CLASS_H

#include "GameObject.h"
#include "Component.h"

class TextureRenderer : public Component
{
public:
	void draw(float deltaTime)
	{

	}

private:
	std::string texturePath;
};

#endif