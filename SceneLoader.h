#ifndef SCENE_LOADER_CLASS_H
#define SCENE_LOADER_CLASS_H

#include "GameObject.h"


class SceneData
{
public:

	SceneData(const std::string& sceneName) : sceneName(sceneName)
	{ }

	void addObject(const GameObject& value)
	{
		sceneObjects.emplace_back(value);
	}

	void addObject(GameObject&& value)
	{
		sceneObjects.emplace_back(std::move(value));
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

	void init()
	{

	}

	void tick()
	{

	}

	void fixedTick()
	{

	}

	void draw()
	{
		for (auto& obj : sceneObjects)
		{
			if (obj->hasComponent<TextureRenderer>())
			{
				//obj->getComponent<TextureRenderer>().draw();
			}
		}
	}

	void load()
	{
		for (auto& obj : data.sceneObjects)
		{
			sceneObjects.emplace_back(obj);
		}
	}

	void unload()
	{
		sceneObjects.clear();
	}

	void reset()
	{
		sceneObjects.clear();
	}

	int getBuildIndex()
	{
		return buildIndex;
	}

private:
	std::vector<std::unique_ptr<GameObject>> sceneObjects;

	int buildIndex;

	SceneData data;

};


#endif