#include "mesh.h"

Mesh::Mesh() : vertices((0, 0)), edges() {

}

Mesh::~Mesh() {
    vertices.resize(0, 0);
}





Eigen::Matrix<float, Dynamic, 3> vertices;
Eigen::Matrix<int, Dynamic, Dynamic> edges;
Eigen::Matrix<float, Dynamic, 2> configParameters; // a and b for each vertex
bool skinnedMesh;
MESTHTYPEID meshId;
