#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "la.h"
#include "raytracing/intersection.h"

class Triangle
{
public:
    Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);
    Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3,
             const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3);
    Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3,
             const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3,
             const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec2 &t3);


    bool Intersect(const glm::vec3& origin, const glm::vec3& direction, Intersection* isect = nullptr) const;
    void InitializeIntersection(Intersection *isect, float t, Point3f pLocal) const;
    void ComputeTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit) const;
    Point2f GetUVCoordinates(const Point3f &point) const;
    Normal3f GetNormal(const Point3f &position) const;
    float Area() const;

    Point3f GetClosestPointToAPoint(const Point3f sourcePosition);

    Point3f points[3];
    Normal3f normals[3];
    Point2f uvs[3];
    Normal3f planeNormal;
};

#endif // TRIANGLE_H



