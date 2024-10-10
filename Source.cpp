#include<iostream>
#include "TextureRenderer.h"
#include "ResourceManager.h"
#include "OvWindow.h"

int main() {
	OvWindow window;
	ResourceManager::registerResource<OvWindow>(window);
	GameObject obj;
	TextureRenderer& renderer = *obj.addComponent<TextureRenderer>();

	window.init();
	
	while (window.windowRunning())
	{
		renderer.draw();
		window.update();
	}
	window.close();
}