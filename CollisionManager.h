#ifndef COLLISION_MANAGER_CLASS_H
#define COLLISION_MANAGER_CLASS_H

#include "PhysicsBody.h"

using namespace glm;
using namespace std;
class CollisionManager {

public:
	static int FindClosestPointOnPoly(vec2 point, vector<vec2> vertices);
	static void ProjectVertices(vector<vec2> vertices, vec2 axis, float& min, float& max);
	static void ProjectCircle(vec2 position, float radius, vec2 axis, float& min, float& max);
	 
	static bool BoundingAABB(Collider& colA, Collider& colB, vec2& mtv);
	static bool BoundingAABB(Collider& colA, Collider& colB);
	static bool BoundingCircle(Collider& colA, Collider& colB, vec2& mtv);
	 
	static bool CheckCollision(Collider& colA, Collider& colB, Manifold& manifold);
	 
	static bool PolyVPoint(vector<vec2> vertices, vec2 point);
	static bool PolyVPoly(PolygonCollider& colA, PolygonCollider& colB, Manifold& manifold);
	static bool PolyVCircle(PolygonCollider& colA, CircleCollider& colB, Manifold& manifold);
	static bool CircleVCircle(CircleCollider& colA, CircleCollider& colB, Manifold& manifold);
	 
	static void GetContactPointsPolyVPoly(PolygonCollider& colA, PolygonCollider& colB, Manifold& manifold);

};

#endif