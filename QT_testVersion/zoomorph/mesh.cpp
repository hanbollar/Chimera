#include "mesh.h"

Mesh::Mesh() : vertices(mat_D3f(0, 0)), edges() {

}

Mesh::~Mesh() {
    vertices.resize(0, 0);
}

void Mesh::loadMesh() {
    return;
}

void Mesh::LBS() {
    return;
}

void Mesh::FFD() {
    return;
}

//Eigen::Matrix<float, Eigen::Dynamic, 3> vertices;
//Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> edges;
//Eigen::Matrix<float, Eigen::Dynamic, 2> configParameters; // a and b for each vertex
bool skinnedMesh;
MESHTYPEID meshId;
