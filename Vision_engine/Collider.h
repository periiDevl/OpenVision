#ifndef COLLIDER_CLASS_H
#define COLLIDER_CLASS_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;


class Collider
{
public:

	/// <summary>
	/// Get Farthest Point In A Certain Direction On The Collider
	/// </summary>
	/// <param name="direction"></param>
	/// <returns>Returns The Farthest Point In The Direction (**IN WORLD POSITION**)</returns>
	virtual vec2 GetSupportPoint(vec2 direction) = 0;
	/// <summary>
	/// A method to calculate the AABB (Max And Min Point) for higher performance and easier collision detection
	/// </summary>
	virtual void CalculateAABB() = 0;
	/// <summary>
	/// A method to calculate the Bound Radius for higher performance and easier collision detection
	/// </summary>
	virtual void CalculateBoundRadius() = 0;
	/// <summary>
	/// The position (x, y) of an object
	/// It is a pointer because you will set it to the body's position that it is being used by 
	/// </summary>
	vec2* Position;
	
	/// <summary>
	/// The rotation (z) of an object
	/// It is a pointer because you will set it to the body's rotation that it is being used by 
	/// </summary>
	float* Rotation;
	
	/// <summary>
	/// The scale (x, y) of an object
	/// It is a pointer because you will set it to the body's scale that it is being used by 
	/// </summary>
	vec2* Scale;

	/// <summary>
	/// Bounding Min Point Relative To Position
	/// </summary>
	vec2 bMin;
	
	/// <summary>
	/// Bounding Max Point Relative To Position
	/// </summary>
	vec2 bMax;
	
	/// <summary>
	/// Bounding Radius, Meaning The Distance To The Farthest Point 
	/// </summary>
	float bRadius;
};

#endif
