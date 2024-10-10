#include<iostream>
#include "TextureRenderer.h"
#include "OvWindow.h"
int main() {
	OvWindow window;
	window.init();

	while (window.windowRunning())
	{
		window.update();
	}
	window.close();
}