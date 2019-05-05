#pragma once
#include <vector>
#include "la.h"
#include "kdtree.h"
#include "triangle.h"
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MDataHandle.h>

// Mesh class that interfaces with our grid structure & voxelization
class Mesh
{
public:
    Mesh();
    Mesh(std::vector<Triangle*> tris);
    void CreateMeshFromMayaMesh(MDataHandle mesh);

    virtual ~Mesh();

    std::vector <Triangle*> faces;
    KDTree* tree = nullptr;
};
