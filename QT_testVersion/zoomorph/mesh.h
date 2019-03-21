#ifndef MESH_H
#define MESH_H

enum {
    BASE,
    ANIMAL
} MESHTYPEID;

class Mesh {
private:
    mat_D3f vertices;
    mat_DDi edges;
    mat_D2f configParameters; // a and b for each vertex pair

    bool skinnedMesh;
    MESTHTYPEID meshId;

public:
    Mesh();
    ~Mesh();

    void loadMesh();
    void LBS();
    void FFD();
};

#endif // MESH_H
