#include "scene/scene.h"

Scene::Scene() : camera(Camera()), all_mesh(std::vector<Mesh*>()) {}

Scene::~Scene() {}

void Scene::SetCamera(const Camera &c) {
    camera = Camera(c);
}

bool Scene::Intersect(const glm::vec3& origin, const glm::vec3& direction, Intersection *isect) const {
    bool result = false;
    for(Mesh* m : all_mesh){
        for (Triangle* t : m->faces) {
            Intersection testIsect;
            if(t->Intersect(origin, direction, &testIsect))
            {
                if(testIsect.t < isect->t || isect->t < 0)
                {
                    *isect = testIsect;
                    result = true;
                }
            }
        }
    }
    return result;
}

void Scene::Clear() {
    camera = Camera();
}
