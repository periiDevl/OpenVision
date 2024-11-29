#include "CollisionManager.h"
#include "Simplex.h"
#include "GJK.h"
#include "EPA.h"

using namespace physics2D;

bool CollisionManager::isCollide(const PhysicsBody& body1, const PhysicsBody& body2, Manifold& manifold)
{
    BaseCollider& collider1 = body1.getCollider();
    BaseCollider& collider2 = body2.getCollider();

    Simplex simplex;
    if (GJK::isTouching(collider1, collider2, simplex) == false)
    {
        return false;
    }

    manifold = EPA::getResolution(collider1, collider2, simplex);

    return true;
}
