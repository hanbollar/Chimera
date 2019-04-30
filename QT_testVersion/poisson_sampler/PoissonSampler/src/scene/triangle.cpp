#include "scene/triangle.h"

Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3):
    Triangle(p1, p2, p3, glm::vec3(1), glm::vec3(1), glm::vec3(1), glm::vec2(0), glm::vec2(0), glm::vec2(0))
{
    for(int i = 0; i < 3; i++)
    {
        normals[i] = planeNormal;
    }
}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3):
    Triangle(p1, p2, p3, n1, n2, n3, glm::vec2(0), glm::vec2(0), glm::vec2(0))
{}


Triangle::Triangle(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &n1, const glm::vec3 &n2, const glm::vec3 &n3, const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec2 &t3)
{
    planeNormal = glm::normalize(glm::cross(p2 - p1, p3 - p2));
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    normals[0] = n1;
    normals[1] = n2;
    normals[2] = n3;
    uvs[0] = t1;
    uvs[1] = t2;
    uvs[2] = t3;
}

float TriArea(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
{
    return glm::length(glm::cross(p1 - p2, p3 - p2)) * 0.5f;
}

//Returns the interpolation of the triangle's three normals based on the point inside the triangle that is given.
Normal3f Triangle::GetNormal(const Point3f &P) const
{
    float A = TriArea(points[0], points[1], points[2]);
    float A0 = TriArea(points[1], points[2], P);
    float A1 = TriArea(points[0], points[2], P);
    float A2 = TriArea(points[0], points[1], P);
    return glm::normalize(normals[0] * A0/A + normals[1] * A1/A + normals[2] * A2/A);
}

void Triangle::InitializeIntersection(Intersection *isect, float t, Point3f pLocal) const
{
    isect->point = pLocal;
    isect->uv = GetUVCoordinates(pLocal);
    ComputeTBN(pLocal, &(isect->normalGeometric), &(isect->tangent), &(isect->bitangent));
    isect->t = t;
}

void Triangle::ComputeTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit) const
{
    *nor = GetNormal(P);
    CoordinateSystem( *nor, tan, bit);
}

Point2f Triangle::GetUVCoordinates(const Point3f &point) const
{
    float A = TriArea(points[0], points[1], points[2]);
    float A0 = TriArea(points[1], points[2], point);
    float A1 = TriArea(points[0], points[2], point);
    float A2 = TriArea(points[0], points[1], point);
    return uvs[0] * A0/A + uvs[1] * A1/A + uvs[2] * A2/A;
}


float Triangle::Area() const
{
    return glm::length(glm::cross(points[0] - points[1], points[2] - points[1])) * 0.5f;
}

bool Triangle::Intersect(const glm::vec3& origin, const glm::vec3& direction, Intersection* isect) const {
    // ray already in local space
    glm::vec3 ray_origin(origin);
    glm::vec3 ray_direction(direction);

    //1. Ray-plane intersection
    float t =  glm::dot(planeNormal, (points[0] - ray_origin)) / glm::dot(planeNormal, ray_direction);
    if(t < 0) return false;

    glm::vec3 P = ray_origin + t * ray_direction;
    //2. Barycentric test
    float S = 0.5f * glm::length(glm::cross(points[0] - points[1], points[0] - points[2]));
    float s1 = 0.5f * glm::length(glm::cross(P - points[1], P - points[2]))/S;
    float s2 = 0.5f * glm::length(glm::cross(P - points[2], P - points[0]))/S;
    float s3 = 0.5f * glm::length(glm::cross(P - points[0], P - points[1]))/S;
    float sum = s1 + s2 + s3;

    if(s1 >= 0.f && s1 <= 1.f && s2 >= 0.f && s2 <= 1.f && s3 >= 0.f && s3 <= 1.f && fequal(sum, 1.f)){
        if (isect == nullptr) {
            return true;
        }
        // otherwise fill in the given intersection
        isect->t = t;
        InitializeIntersection(isect, t, Point3f(P));
        return true;
    }
    return false;
}

Point3f Triangle::GetClosestPointToAPoint(const Point3f sourcePosition)
{
    Vector3f edge0 = points[1] - points[0];
    Vector3f edge1 = points[2] - points[0];
    Vector3f v0 = points[0] - sourcePosition;

        float a = glm::dot(edge0, edge0);
        float b =  glm::dot(edge0, edge1);
        float c =  glm::dot(edge1, edge1);
        float d =  glm::dot(edge0, v0);
        float e = glm::dot(edge1, v0);

        float det = a*c - b*b;
        float s = b*e - c*d;
        float t = b*d - a*e;

        if ( s + t < det )
        {
            if ( s < 0.f )
            {
                if ( t < 0.f )
                {
                    if ( d < 0.f )
                    {
                        s = glm::clamp( -d/a, 0.f, 1.f );
                        t = 0.f;
                    }
                    else
                    {
                        s = 0.f;
                        t = glm::clamp( -e/c, 0.f, 1.f );
                    }
                }
                else
                {
                    s = 0.f;
                    t = glm::clamp( -e/c, 0.f, 1.f );
                }
            }
            else if ( t < 0.f )
            {
                s = glm::clamp( -d/a, 0.f, 1.f );
                t = 0.f;
            }
            else
            {
                float invDet = 1.f / det;
                s *= invDet;
                t *= invDet;
            }
        }
        else
        {
            if ( s < 0.f )
            {
                float tmp0 = b+d;
                float tmp1 = c+e;
                if ( tmp1 > tmp0 )
                {
                    float numer = tmp1 - tmp0;
                    float denom = a-2*b+c;
                    s = glm::clamp( numer/denom, 0.f, 1.f );
                    t = 1-s;
                }
                else
                {
                    t = glm::clamp( -e/c, 0.f, 1.f );
                    s = 0.f;
                }
            }
            else if ( t < 0.f )
            {
                if ( a+d > b+e )
                {
                    float numer = c+e-b-d;
                    float denom = a-2*b+c;
                    s = glm::clamp( numer/denom, 0.f, 1.f );
                    t = 1-s;
                }
                else
                {
                    s = glm::clamp( -e/c, 0.f, 1.f );
                    t = 0.f;
                }
            }
            else
            {
                float numer = c+e-b-d;
                float denom = a-2*b+c;
                s = glm::clamp( numer/denom, 0.f, 1.f );
                t = 1.f - s;
            }
        }

        return points[0] + s * edge0 + t * edge1;
}
