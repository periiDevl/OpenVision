#ifndef COMPONENT_CLASS_H
#define COMPONENT_CLASS_H

class GameObject;

class Component 
{
public:
	virtual ~Component() = default;

	Component(GameObject& owner, bool initState) : gameObject(owner), isActive(initState)
	{ }
	Component(GameObject& owner) : gameObject(owner), isActive(true)
	{ }

/*	virtual void init();
	
	virtual void tick(double deltaTime);

	virtual void fixedTick();
*/
	void setActive(bool state)
	{
		isActive = state;
	}
	bool getActive()
	{
		return isActive;
	}

protected:

	bool isActive; // state
	GameObject& gameObject; // owner

};




#endif
