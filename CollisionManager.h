#ifndef COLLISION_MANAGER_CLASS_H
#define COLLISION_MANAGER_CLASS_H

#include "PhysicsBody.h"

using namespace glm;
using namespace std;

/// <summary>
/// A functions for checking AABB collision (Axis-Aligned-Bounding-Box) between 2 colliders
/// This is an easy way that costs a lot less to calculate, it just defines 2 boxes with a minimum and maximum point of each shape and checks if the boxes collide.
/// You will use such a function before a real collision detection test to speed things up, and not waste performance.
/// </summary>
/// <param name="colA">The First Collider</param>
/// <param name="colB">The Second Collider</param>
/// <param name="mtv">Minimum Translation Vector</param>
/// <returns></returns>
bool BoundingAABB(Collider& colA, Collider& colB, vec2& mtv);
/// <summary>
/// A functions for checking Bounding Circle Collision between 2 colliders
/// This is an easy way that costs a lot less to calculate, it just defines 2 circles for the whole shape and checks if the circle is colliding.
/// You will use such a function before a real collision detection test to speed things up, and not waste performance.
/// </summary>
/// <param name="colA">The First Collider</param>
/// <param name="colB">The Second Collider</param>
/// <param name="mtv">Minimum Translation Vector</param>
/// <returns></returns>
bool BoundingCircle(const Collider& colA, const Collider& colB, vec2& mtv);
/// <summary>
/// A functions for checking Polygon V Polygon Collision
/// It uses the SAT (Seperating Axis Theorem) algorithm for configuring out all the "normals" of a polygon VS the other, and tries to find a line of seperation, which if founded, proves there is no collision between the objects.
/// </summary>
/// <param name="colA">The First Collider</param>
/// <param name="colB">The Second Collider</param>
/// <param name="mtv">Minimum Translation Vector</param>
/// <returns></returns>
bool PolygonVPolygon(const PolygonCollider& colA, const PolygonCollider& colB, vec2& mtv);
bool PolygonVCircle(const PolygonCollider& colA, const CircleCollider& colB, vec2& mtv);
bool CircleVCircle(const CircleCollider& colA, const CircleCollider& colB, vec2& mtv);


#endif