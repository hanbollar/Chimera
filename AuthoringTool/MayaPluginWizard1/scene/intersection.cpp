#include "intersection.h"

Intersection::Intersection():
    point(Point3f(0)),
    normalGeometric(Normal3f(0)),
    uv(Point2f(0)),
    t(INFINITY),
    tangent(0.f), bitangent(0.f)
{}
