#ifndef COLLIDER_H_CLASS
#define COLLIDER_H_CLASS

#include "Component.h"
#include "BaseCollider.h"

namespace physics2D
{
	class Collider : public Component
	{
		using Component::Component;
	public:
		Collider(GameObject& owner, BaseCollider collider) : Component(owner),
			m_collider(glm::vec2(0, 0))
		{
			m_collider = collider;
		}

	private:
		BaseCollider m_collider;
	};

}
#endif