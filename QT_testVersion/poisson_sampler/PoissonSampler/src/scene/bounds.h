#pragma once
#include <globals.h>
#include "openGL/drawable.h"
#include "mesh.h"

class Bounds3f {
public:
    glm::vec3 min, max;

    Bounds3f();
    Bounds3f(const glm::vec3& min, const glm::vec3& max);
    Bounds3f(const glm::vec3& p);
    Bounds3f(const Bounds3f& b);

    Bounds3f(const std::vector<Triangle*>& t);

    ~Bounds3f();

    static Bounds3f* BuildBoundingBox(const std::vector<Triangle*>& t);
    static Bounds3f Union(const Bounds3f& b1, const Bounds3f& b2);
    static Bounds3f Union(const Bounds3f& b1, const glm::vec3& p);

//    virtual void drawMode();
//    virtual void create();

    bool Intersect(const glm::vec3& origin, const glm::vec3& direction, float* t) const;
};


