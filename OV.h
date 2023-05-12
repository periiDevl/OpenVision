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

	
};

#endif
