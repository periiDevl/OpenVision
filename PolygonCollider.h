#ifndef POLYGON_COLLIDER_CLASS_H
#define POLYGON_COLLIDER_CLASS_H

#include "Collider.h"
#include <glad/glad.h>
#include <map>


class PolygonCollider : public Collider
{
public:
	/// <summary>
	/// A constructor for the Polygon Collider
	/// </summary>
	/// <param name="pos">The Address In Memory of the position variable (vec2)</param>
	/// <param name="rot">The Address In Memory of the rotation variable (float)</param>
	/// <param name="sca">The Address In Memory of the scale variable (vec2)</param>
	/// <param name="vertices">The Address In Memory of the vertices variable (vector<vec2>)</param>
	PolygonCollider(vec2* pos, float* rot, vec2* sca, vector<vec2> vertices);
	/// <summary>
	/// A constructor for the Polygon Collider
	/// </summary>
	/// <param name="pos">The Address In Memory of the position variable (vec2)</param>
	/// <param name="rot">The Address In Memory of the rotation variable (float)</param>
	/// <param name="sca">The Address In Memory of the scale variable (vec2)</param>
	PolygonCollider(vec2* pos, float* rot, vec2* sca);
	/// <summary>
	/// Calculate the Vertices translated (moved), rotated and scaled by the collider's transform
	/// </summary>
	/// <returns>A vector of vec2 of the vertices (vector<vec2>)</returns>
	vector<vec2> GetTransformedVertices();
	/// <summary>
	/// A method to calculate the AABB (Max And Min Point) for higher performance and easier collision detection
	/// </summary>
	void CalculateAABB();
	/// <summary>
	/// A method to calculate the Bound Radius for higher performance and easier collision detection
	/// </summary>
	void CalculateBoundRadius();
	/// <summary>
	/// All the vertices of the polygon
	/// </summary>
	vector<vec2> vertices;
};

#endif
