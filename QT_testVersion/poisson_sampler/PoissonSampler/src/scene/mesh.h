#pragma once
#include <QList>
#include <QDateTime>

#include "la.h"
#include "raytracing/intersection.h"
#include "openGL/drawable.h"
#include "tinyobj/tiny_obj_loader.h"
#include "kdtree.h"
#include "triangle.h"

//A mesh's primary purpose is to store VBOs for rendering the triangles in OpenGL.
class Mesh : public Drawable
{
public:
    Mesh();
    Mesh(std::vector<Triangle*> tris);
    virtual ~Mesh();
    virtual void create();
    void LoadOBJ(const QStringRef &filename, const QStringRef &local_path, const Transform& transform);
    virtual GLenum drawMode() const;

    std::vector<Triangle*> faces;
    KDTree* tree = nullptr;
};

class COM : public Drawable
{
public:
    virtual void create();
    virtual GLenum drawMode() const;

    glm::vec3 loc;
};






