#pragma once
#include <maya/MPxNode.h>
#include <scene/mesh.h>
#include <scene/grid.h>

class ZoomorphNode : public MPxNode
{
public:
    ZoomorphNode();
    virtual ~ZoomorphNode();
    virtual MStatus compute(const MPlug& plug, MDataBlock& data);
    static void* creator();
    static MStatus initialize();
    static MTypeId id;

	static MObject animalMesh;
	static MObject baseMesh;
	static MObject outputMesh;
    static MObject gridSize;
	static MObject weighting;
    static MObject comEnergy;

protected:
	MObject createMesh(Mesh* ZoomorphMesh, MObject& outData, MStatus& stat);
    MObject createCubeMesh(MObject& outData, MStatus& stat);

    Mesh* getZoomorphMesh(MDataHandle animalM, MDataHandle baseM, double w, int gridSize, float& com);
    glm::vec3 ZoomorphNode::CenterOfMassEnergy(Mesh* mesh_source);
};

