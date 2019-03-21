#ifndef BASEMESH_H
#define BASEMESH_H

#include "mesh.h"

class BaseMesh : public Mesh {
private:

public:
    BaseMesh();
    ~BaseMesh();

    // using alpha and beta values
    // we have current configParameters
    mat_D2f VDR();
};

#endif // BASEMESH_H
