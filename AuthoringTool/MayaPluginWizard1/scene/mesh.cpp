#include "mesh.h"
#include <io.h>
#include <maya/MGlobal.h>
#include <maya/MStreamUtils.h>
#include <maya/MFloatPointArray.h>
#include <maya/MDagPath.h>

Mesh::Mesh()
{}
Mesh::Mesh(std::vector<Triangle*> tris) : faces(tris), tree(new KDTree(tris))
{}
Mesh::~Mesh() {
    faces.clear();
}


void Mesh::CreateMeshFromMayaMesh(MDataHandle meshData) {
    MFnMesh mesh = meshData.asMesh();

    MFloatPointArray points;

    mesh.getPoints(points, MSpace::kWorld);
    MString allpoints = "";

    MIntArray triangleVertices;
    MIntArray triangleCounts;

    mesh.getTriangles(triangleCounts, triangleVertices);

    MStreamUtils::stdOutStream() << "TRIANGLE COUNTS" << "\n";
    for (unsigned int i = 0; i < triangleCounts.length(); i++)
    {
        MStreamUtils::stdOutStream() << triangleCounts[i] << "\n";
    }

    MStreamUtils::stdOutStream() << "TRIANGLE VERTICES" << "\n";
    for (unsigned int i = 0; i < triangleVertices.length(); i++)
    {
        MStreamUtils::stdOutStream() << triangleVertices[i] << "\n";
    }

    MStreamUtils::stdOutStream() << "ALL POINTS" << "\n";
    for (unsigned int i = 0; i < points.length(); i++)
    {
        MStreamUtils::stdOutStream() << points[i].x << " " << points[i].y << 
            " " << points[i].z << "\n";
    }

    for (unsigned int i = 0; i < triangleVertices.length(); i+=3)
    {
        int index1 = triangleVertices[i];
        int index2 = triangleVertices[i + 1];
        int index3 = triangleVertices[i + 2];
        Point3f p1(points[index1].x, points[index1].y, points[index1].z);
        Point3f p2(points[index2].x, points[index2].y, points[index2].z);
        Point3f p3(points[index3].x, points[index3].y, points[index3].z);

        auto t = new Triangle(p1, p2, p3);

        this->faces.push_back(t);
    }

    this->tree = new KDTree(faces);
}
