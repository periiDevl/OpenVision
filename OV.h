#ifndef OV_H
#define OV_H
#include"Object.h"
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

	
};

#endif
