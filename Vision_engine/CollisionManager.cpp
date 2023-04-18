#include "CollisionManager.h"

bool BoundingAABB(const Collider& colA, const Collider& colB, vec2& mtv)
{

    return false;
}

bool BoundingCircle(const Collider& colA, const Collider& colB, vec2& mtv) {
    return glm::distance(*colA.Position, *colB.Position) <= colA.bRadius + colB.bRadius;
}

bool PolygonVPolygon(const PolygonCollider& colA, const PolygonCollider& colB, vec2& mtv) {
    float overlap = INFINITY;
    vec2 smallestOverlapAxis;

    vector<vec2> verticesA = colA.GetTransformedVertices();
    vector<vec2> verticesB = colB.GetTransformedVertices();

    

    // Loop over all edges of both polygons
    for (int i = 0; i < verticesA.size(); i++) {
        int j = (i + 1) % verticesA.size();

        vec2 edge = verticesA[j] - verticesA[i];
        vec2 axis(-edge.y, edge.x);
        axis = normalize(axis);

        // Project vertices of both polygons onto axis
        float minA = INFINITY, maxA = -INFINITY;
        float minB = INFINITY, maxB = -INFINITY;

        for (int k = 0; k < verticesA.size(); k++) {
            float projection = dot(verticesA[k], axis);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        for (int k = 0; k < verticesB.size(); k++) {
            float projection = dot(verticesB[k], axis);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        // Check if projections overlap
        if (maxA < minB || maxB < minA) {
            return false;
        }
        else {
            float overlapAxis = std::min(maxA, maxB) - std::max(minA, minB);
            if (overlapAxis < overlap) {
                overlap = overlapAxis;
                smallestOverlapAxis = axis;
            }
        }
    }

    // Loop over all edges of both polygons
    for (int i = 0; i < verticesB.size(); i++) {
        int j = (i + 1) % verticesB.size();

        vec2 edge = verticesB[j] - verticesB[i];
        vec2 axis(-edge.y, edge.x);
        axis = normalize(axis);

        // Project vertices of both polygons onto axis
        float minA = INFINITY, maxA = -INFINITY;
        float minB = INFINITY, maxB = -INFINITY;

        for (int k = 0; k < verticesA.size(); k++) {
            float projection = dot(verticesA[k], axis);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        for (int k = 0; k < verticesB.size(); k++) {
            float projection = dot(verticesB[k], axis);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        // Check if projections overlap
        if (maxA < minB || maxB < minA) {
            return false;
        }
        else {
            float overlapAxis = std::min(maxA, maxB) - std::max(minA, minB);
            if (overlapAxis < overlap) {
                overlap = overlapAxis;
                smallestOverlapAxis = axis;
            }
        }
    }
    // If we got here, the polygons are intersecting
    mtv = smallestOverlapAxis * overlap;

    // 
    vec2 posDif = vec2(*colB.Position) - vec2(*colA.Position);
    if (dot(mtv, posDif) > 0)
        mtv = -mtv;

    return true;
}

bool PolygonVCircle(const PolygonCollider& colA, const CircleCollider& colB, vec2& mtv) {
    float overlap = 0.0f;

    vector<vec2> vertices = colA.GetTransformedVertices();
    float minOverlap = FLT_MAX;

    for (int i = 0; i < vertices.size(); i++) {

        // Calculate the next index
        int j = (i + 1) % vertices.size();

        // Calculate the edge
        vec2 edge = (vertices)[j] - (vertices)[i];

        // The normal from the edge (normalized)
        vec2 normal = normalize(vec2(-edge.y, edge.x));

        // Project the vertices of the polygon onto the normal
        float minA = FLT_MAX;
        float maxA = -FLT_MAX;
        for (int k = 0; k < vertices.size(); k++) {
            float projection = dot(vertices[k], normal);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        // Project the center of the circle onto the normal
        float centerProjection = dot(*colB.Position, normal);

        // Calculate the overlap between the projections
        float distance = centerProjection - maxA;
        if (distance > 0) {
            // The circle is completely outside the polygon
            return false;
        }
        else {
            overlap = -distance;
            if (overlap < minOverlap) {
                // Update the minimum overlap and MTV
                minOverlap = overlap;
                mtv = normal * overlap;
            }
        }
    }
    // Check for overlap between the circle and the polygon vertices
    for (int i = 0; i < vertices.size(); i++) {
        float distance = length(*colB.Position - vertices[i]);
        if (distance < colB.radius) {
            // There is an overlap between the circle and the polygon vertex
            vec2 direction = normalize(*colB.Position - vertices[i]);
            float overlap = colB.radius - distance;
            if (overlap < minOverlap) {
                // Update the minimum overlap and MTV
                minOverlap = overlap;
                mtv = direction * overlap;
            }
        }
    }
    return true;
}

bool CircleVCircle(const CircleCollider& colA, const CircleCollider& colB, vec2& mtv) {
    return glm::distance(*colA.Position, *colB.Position) <= colA.radius + colB.radius;
}
