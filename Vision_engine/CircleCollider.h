#ifndef CIRCLE_COLLIDER_CLASS_H
#define CIRCLE_COLLIDER_CLASS_H

#include "Collider.h"
#include <glad/glad.h>


class CircleCollider : public Collider
{
public:
	
	/// <summary>
	/// A constructor for the Circle Collider
	/// </summary>
	/// <param name="pos">The Address In Memory of the position variable (vec2)</param>
	/// <param name="rot">The Address In Memory of the rotation variable (float)</param>
	/// <param name="sca">The Address In Memory of the scale variable (vec2)</param>
	CircleCollider(vec2* pos, float* rot, vec2* sca);

	/// <summary>
	/// Get Farthest Point In A Certain Direction On The Collider
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>Returns The Farthest Point In The Direction (**IN WORLD POSITION**)</returns>
	vec2 GetSupportPoint(vec2 direction);
	
	/// <summary>
	/// A method to calculate the AABB (Max And Min Point) for higher performance and easier collision detection
	/// </summary>
	void CalculateAABB();
	
	/// <summary>
	/// A method to calculate the Bound Radius for higher performance and easier collision detection
	/// </summary>
	void CalculateBoundRadius();
	
	/// <summary>
	/// The Radius Of The Circle Collider 
	/// </summary>
	float radius;
};

#endif
