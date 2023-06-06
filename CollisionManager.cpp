#include "CollisionManager.h"
vec2 perpendicular(vec2 v) { vec2 p = { v.y, -v.x }; return p; }
float dotProduct(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
float lengthSquared(vec2 v) { return v.x * v.x + v.y * v.y; }
vec2 tripleProduct(vec2 a, vec2 b, vec2 c) {

    vec2 r;

    float ac = a.x * c.x + a.y * c.y; // perform a.dot(c)
    float bc = b.x * c.x + b.y * c.y; // perform b.dot(c)

    // perform b * a.dot(c) - a * b.dot(c)
    r.x = b.x * ac - a.x * bc;
    r.y = b.y * ac - a.y * bc;
    return r;
}
bool BoundingAABB(Collider& colA, Collider& colB, vec2& mtv)
{
    colA.CalculateAABB();
    colB.CalculateAABB();

    glm::vec2 aMin = colA.bMin;  // Minimum point of AABB A
    glm::vec2 aMax = colA.bMax;  // Maximum point of AABB A
    glm::vec2 bMin = colB.bMin;  // Minimum point of AABB B
    glm::vec2 bMax = colB.bMax;  // Maximum point of AABB B

    // Calculate the MTV
    float overlapX = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
    float overlapY = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);

    if (overlapX < 0 || overlapY < 0) {
        // No collision occurred
        mtv = vec2(0, 0);
        return false;
    }

    // Determine which axis has the smaller overlap
    if (overlapX < overlapY) {
        mtv = vec2(overlapX, 0);
        if ((aMax.x + aMin.x) / 2 < (bMax.x + bMin.x) / 2) {
            mtv.x = -mtv.x;
        }
    }
    else {
        mtv = vec2(0, overlapY);
        if ((aMax.y + aMin.y) / 2 >= (bMax.y + bMin.y) / 2) {
            mtv.y = -mtv.y;
        }
    }

    return true;
}
bool BoundingAABB(Collider& colA,Collider& colB)
{
    colA.CalculateAABB();
    colB.CalculateAABB();

    glm::vec2 aMin = colA.bMin; 
    glm::vec2 aMax = colA.bMax; 
    glm::vec2 bMin = colB.bMin; 
    glm::vec2 bMax = colB.bMax; 

    float overlapX = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
    float overlapY = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);

    if (overlapX < 0 || overlapY < 0) {
        return false;
    }

    return true;
}
bool BoundingCircle(Collider& colA, Collider& colB, vec2& mtv) {
    if (glm::distance(*colA.Position, *colB.Position) <= colA.bRadius + colB.bRadius) {
        mtv = glm::normalize(*colA.Position - *colB.Position) * (colA.bRadius + colB.bRadius - glm::distance(*colA.Position, *colB.Position));
        return true;
    }

    return false;
}

bool CheckCollision(Collider& colA, Collider& colB, vec2& mtv)
{
    if (BoundingAABB(colA, colB)) {
        if ((colA.type == ColliderType::Polygon || colA.type == ColliderType::Box) && (colB.type == ColliderType::Polygon || colB.type == ColliderType::Box)) {
            
            PolygonCollider& polyA = dynamic_cast<PolygonCollider&>(colA);
            PolygonCollider& polyB = dynamic_cast<PolygonCollider&>(colB);
            
            return PolyVPoly(polyA, polyB, mtv);
        }
        else if ((colA.type == ColliderType::Polygon || colA.type == ColliderType::Box) && (colB.type == ColliderType::Circle)) {
            
            PolygonCollider& polyA = dynamic_cast<PolygonCollider&>(colA);
            CircleCollider& circleB = dynamic_cast<CircleCollider&>(colB);
            
            return PolyVCircle(polyA, circleB, mtv);
        }
        else if ((colA.type == ColliderType::Circle) && (colB.type == ColliderType::Polygon || colB.type == ColliderType::Box)) {
            
            CircleCollider& circleA = dynamic_cast<CircleCollider&>(colA);
            PolygonCollider& polyB = dynamic_cast<PolygonCollider&>(colB);
            
            return PolyVCircle(polyB, circleA, mtv);
        }
        else if ((colA.type == ColliderType::Circle) && (colB.type == ColliderType::Circle)) {
            
            CircleCollider& circleA = dynamic_cast<CircleCollider&>(colA);
            CircleCollider& circleB = dynamic_cast<CircleCollider&>(colB);
            
            return CircleVCircle(circleA, circleB, mtv);
        }
    }
    return false;
}

int FindClosestPointOnPoly(vec2 point, vector<vec2> vertices)
{
    int result = -1;
    float minDistance = INFINITY;

    for (int i = 0; i < vertices.size(); i++) {
        vec2 v = vertices[i];
        float Distance = lengthSquared(v - point);

        if (Distance < minDistance) {
            minDistance = Distance;
            result = i;
        }
    }

    return result;
}
void ProjectCircle(vec2 position, float radius, vec2 axis, float& min, float& max) {
    vec2 dir = normalize(axis);
    vec2 dirAndRadius = dir * radius;

    vec2 point1 = position + dirAndRadius;
    vec2 point2 = position - dirAndRadius;

    min = dot(point1, axis);
    max = dot(point2, axis);

    if (min > max) {
        std::swap(min, max);
    }
}
void ProjectVertices(vector<vec2> vertices, vec2 axis, float& min, float& max) {
    min = INFINITY;
    max = -INFINITY;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        vec2 v = vertices[i];
        float proj = dot(v, axis);

        if (proj < min)
            min = proj;
        if (proj > max)
            max = proj;
    }

}
bool PolyVPoly(PolygonCollider& colA, PolygonCollider& colB, vec2& mtv) {
    vec2 normal;
    float depth = INFINITY;
    vector<vec2> verticesA = colA.GetTransformedVertices();
    vector<vec2> verticesB = colB.GetTransformedVertices();
    for (size_t i = 0; i < verticesA.size(); i++)
    {
        vec2 vA = verticesA[i];
        vec2 vB = verticesA[(i + 1) % verticesA.size()];

        vec2 edge = vB - vA;
        vec2 checkAxis = normalize(vec2(-edge.y, edge.x));

        float minA, maxA;
        float minB, maxB;
        ProjectVertices(verticesA, checkAxis, minA, maxA);
        ProjectVertices(verticesB, checkAxis, minB, maxB);

        if (minA >= maxB || minB >= maxA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);

        if (axisDepth < depth)
        {
            depth = axisDepth;
            normal = checkAxis;
        }
    }
    for (size_t i = 0; i < verticesB.size(); i++)
    {
        vec2 vA = verticesB[i];
        vec2 vB = verticesB[(i + 1) % verticesB.size()];

        vec2 edge = vB - vA;
        vec2 checkAxis = normalize(vec2(-edge.y, edge.x));

        float minA, maxA;
        float minB, maxB;
        ProjectVertices(verticesA, checkAxis, minA, maxA);
        ProjectVertices(verticesB, checkAxis, minB, maxB);

        if (minA >= maxB || minB >= maxA) {
            return false;
        }

        float axisDepth = std::min(maxB - minA, maxA - minB);

        if (axisDepth < depth)
        {
            depth = axisDepth;
            normal = checkAxis;
        }
    }

    normal = glm::rotate(normal, radians(*colA.Rotation));
    normal = glm::rotate(normal, -radians(*colB.Rotation));


    vec2 direction = *colB.Position - *colA.Position;

    if (dot(direction, normal) > 0) {
        normal = -normal;
    }
    
    mtv = normal * depth;
    return true;
}


bool PolyVCircle(PolygonCollider& colA, CircleCollider& colB, vec2& mtv) {
    vec2 normal;
    float depth = INFINITY;
    
    vec2 checkAxis;
    float axisDepth;

    float minA, maxA;
    float minB, maxB;


    vector<vec2> vertices = colA.GetTransformedVertices();

    for (size_t i = 0; i < colA.vertices.size(); i++)
    {
        vec2 vA = vertices[i];
        vec2 vB = vertices[(i + 1) % vertices.size()];

        vec2 edge = vB - vA;
        checkAxis = normalize(vec2(-edge.y, edge.x));

        ProjectVertices(vertices, checkAxis, minA, maxA);
        ProjectCircle(*colB.Position, colB.radius, checkAxis, minB, maxB);

        if (minA >= maxB || minB >= maxA)
            return false;

        axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth);
        {
            depth = axisDepth;
            normal = checkAxis;
        }
    }

    int pIndex = FindClosestPointOnPoly(*colB.Position, vertices);
    vec2 point = vertices[pIndex];

    checkAxis = normalize(point - *colB.Position);

    ProjectVertices(vertices, checkAxis, minA, maxA);
    ProjectCircle(*colB.Position, colB.radius, checkAxis, minB, maxB);

    if (minA >= maxB || minB >= maxA) {
        return false;
    }

    axisDepth = std::min(maxB - minA, maxA - minB);

    if (axisDepth < depth) {
        depth = axisDepth;
        normal = checkAxis;
    }

    vec2 direction = *colA.Position - *colB.Position;
    if (dot(direction, normal) > 0)
        normal = -normal;

    mtv = normal * depth;

    return true;

}

bool CircleVCircle(CircleCollider& colA, CircleCollider& colB, vec2& mtv) {
    return glm::distance(*colA.Position, *colB.Position) <= colA.radius + colB.radius;
}
