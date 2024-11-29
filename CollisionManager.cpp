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
	// Get bodies
    PhysicsBody& body1 = *manifold.body1;
    PhysicsBody& body2 = *manifold.body2;

	// For now elasticity is constant
	float elasticity = .5;

	glm::vec2 relVel = body1.velocity() - body2.velocity();

	// inverse mass (for static objects)
	float invMass1 = body1.isStatic() ? 0.0f : body1.invMass();
	float invMass2 = body2.isStatic() ? 0.0f : body2.invMass();

	// Calculating impulse magnitude
	// Physics Formula:
	// J = -(1 + e) * (relativeVelocity * normal) / (1/mass1 + 1/mass2)
	float impulseMagnitude =
		(-(1 + elasticity) * glm::dot(relVel, manifold.normal))
		/ (invMass1 + invMass2);

	glm::vec2 impulse = manifold.normal * impulseMagnitude;

	body1.applyImpulse(impulse);
	body2.applyImpulse(-impulse);
}
