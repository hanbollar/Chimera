#ifndef MESH_H
#define MESH_H

#include "globals.h"

enum MESHTYPEID {
    BASE,
    ANIMAL
};

class Mesh {
private:
    mat_D3f vertices;
    mat_DDi edges;
    mat_D2f configParameters; // a and b for each vertex pair

    bool skinnedMesh;
    MESHTYPEID meshId;

public:
    Mesh();
    ~Mesh();

    void loadMesh();
    void LBS();
    void FFD();
};

#endif // MESH_H
