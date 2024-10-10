#ifndef COMPONENT_CLASS_H
#define COMPONENT_CLASS_H

class GameObject;

class Component
{
public:
	virtual ~Component() = default;

	void setActive(bool state)
	{
		isActive = state;
	}
	bool getActive()
	{
		return isActive ;
	}

protected:
	Component(GameObject& owner, bool initState) : gameObject(owner), transform(owner.transform), isActive(initState)
	{ }
	Component(GameObject& owner) : gameObject(owner), transform(owner.transform), isActive(true)
	{ }
	
	virtual void init() = 0;
	virtual void fixedTick() = 0;
	virtual void tick(float deltaTime) = 0;

	bool isActive; // state
	GameObject& gameObject; // owner
	Transform& transform; // owner

};




#endif
