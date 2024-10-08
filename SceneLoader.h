#ifndef SCENE_LOADER_CLASS_H
#define SCENE_LOADER_CLASS_H

#include "GameObject.h"


class SceneData
{
public:

	// a list of copies of game objects (will be copied when init scene)
	std::vector<GameObject> sceneObjects;

	int buildIndex;
	std::string sceneName;
	std::string relativePath;
};


class Scene
{
public:



private:
	std::vector<std::unique_ptr<GameObject>> sceneObjects;

	SceneData data;

};


#endif