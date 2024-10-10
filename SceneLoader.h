#ifndef SCENE_LOADER_CLASS_H
#define SCENE_LOADER_CLASS_H

#include "GameObject.h"


class SceneData
{
public:

	SceneData(std::string xmlPath)
	{

	}

	// a list of copies of game objects (will be copied when init scene)
	std::vector<GameObject> sceneObjects;

	int buildIndex;
	std::string sceneName;
	std::string relativePath;
};


class Scene
{

public:

	void load()
	{

	}

	void unload()
	{

	}

	void reset()
	{
		sceneObjects.clear();
	}


private:
	std::vector<std::unique_ptr<GameObject>> sceneObjects;

	SceneData data;

};


#endif