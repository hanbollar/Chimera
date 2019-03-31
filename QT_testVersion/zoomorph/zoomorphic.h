#ifndef ZOOMORPHIC_H
#define ZOOMORPHIC_H

#include "mesh.h"
#include "correspondance.h"

class Zoomorph {
private:
    Correspondance bestC;
    Mesh* animalMesh;
    Mesh* baseMesh;
public:
    Zoomorph();
    ~Zoomorph();

    void refineConfig();
    void remove();
    void smooth();
    void unionTogether();
    Mesh* convertToMesh();
};

#endif // ZOOMORPHIC_H
