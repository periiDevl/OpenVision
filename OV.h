#ifndef OV_H
#define OV_H
#include"Object.h"
#include <fstream>
#include <iostream>
#include <filesystem>
class OV {
public:
	static Object* SearchObjectByName(std::string Name,std::vector<Object>& sceneObjects) {

        for (int i = 0; i < sceneObjects.size(); i++) {
            if (sceneObjects[i].name == Name)
				return &sceneObjects[i];
			
        }
		return &sceneObjects[0];
	}

	static void SetTexture(std::string Path, Object& obj)
	{
		obj.tex = Texture(Path.c_str());
	}
	



	

	glm::vec2 getImageAspectRatio(const char* filename) {
		int width, height, channels;
		stbi_uc* imageData = stbi_load(filename, &width, &height, &channels, 0);

		if (imageData != nullptr) {
			float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
			stbi_image_free(imageData);
			return glm::vec2(aspectRatio, 1.0f);
		}
		else {
			std::cout << "Failed to load image." << std::endl;
			return glm::vec2(0.0f);
		}
	}
	
};

#endif
