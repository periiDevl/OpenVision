#ifndef COLLISION_MANAGER_CLASS_H
#define COLLISION_MANAGER_CLASS_H

#include "PhysicsBody.h"

using namespace glm;
using namespace std;

int FindClosestPointOnPoly(vec2 point, vector<vec2> vertices);
void ProjectVertices(vector<vec2> vertices, vec2 axis, float& min, float& max);

bool BoundingAABB(Collider& colA, Collider& colB, vec2& mtv);
bool BoundingAABB(Collider& colA, Collider& colB);
bool BoundingCircle(const Collider& colA, const Collider& colB, vec2& mtv);

bool CheckCollision(Collider& colA, Collider& colB, Manifold& manifold);

bool PolyVPoly(PolygonCollider& colA, PolygonCollider& colB, Manifold& manifold);
bool PolyVCircle(PolygonCollider& colA, CircleCollider& colB, Manifold& manifold);
bool CircleVCircle(CircleCollider& colA, CircleCollider& colB, Manifold& manifold);

void GetContactPointsPolyVPoly(PolygonCollider& colA, PolygonCollider& colB, Manifold& manifold);


#endif