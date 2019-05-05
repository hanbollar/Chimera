#pragma once
#include <globals.h>

class Intersection
{
public:
    Intersection();

    Point3f point;          // The place at which the intersection occurred
    Normal3f normalGeometric; // The surface normal at the point of intersection, NO alterations like normal mapping applied
    Point2f uv;             // The UV coordinates computed at the intersection
    float t;                  // The parameterization for the ray (in world space) that generated this intersection.
                              // t is equal to the distance from the point of intersection to the ray's origin if the ray's direction is normalized.

    Vector3f tangent, bitangent; // World-space vectors that form an orthonormal basis with the surface normal.
};
