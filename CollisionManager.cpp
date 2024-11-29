#include "CollisionManager.h"

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

void CollisionManager::resolveManifold(Manifold& manifold)
{
    PhysicsBody& body1 = *manifold.body1;
    PhysicsBody& body2 = *manifold.body2;

    // For now full elastic materials
    float elasticity = 1;

    glm::vec2 relVel = body1.velocity() - body2.velocity();

    float impulseMagnitude = 
        (-elasticity * glm::dot(relVel, manifold.normal) - glm::dot(relVel, manifold.normal)) 
        / (body1.invMass() + body2.invMass());
    glm::vec2 impulseDirection = manifold.normal;

    glm::vec2 impulse = impulseDirection * impulseMagnitude;

    body1.applyForce(-impulse);
    body2.applyForce( impulse);
}
