#pragma once
#include <vector>

#include "scene/camera.h"
#include "scene/mesh.h"

class Mesh;

class Scene {
public:
    Scene();
    ~Scene();

    Camera camera;
    std::vector<Mesh*> all_mesh;

    void SetCamera(const Camera &c);

    void CreateTestScene();
    void Clear();

    bool Intersect(const glm::vec3& origin, const glm::vec3& direction, Intersection *isect) const;
};
