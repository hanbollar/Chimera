#include "ZoomorphNode.h"
#include <maya/MGlobal.h>
#include <maya/MStatus.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatPointArray.h>
#include <maya/MStreamUtils.h>

// Attributes
MObject ZoomorphNode::animalMesh;
MObject ZoomorphNode::baseMesh;
MObject ZoomorphNode::outputMesh;
MObject ZoomorphNode::gridSize;
MObject ZoomorphNode::weighting;
MObject ZoomorphNode::comEnergy;

MTypeId ZoomorphNode::id(0x0);

#define MAKE_INPUT(attr) \
CHECK_MSTATUS(attr.setKeyable(true)); \
CHECK_MSTATUS(attr.setStorable(true)); \
CHECK_MSTATUS(attr.setReadable(true)); \
CHECK_MSTATUS(attr.setWritable(true));

#define MAKE_OUTPUT(attr) \
CHECK_MSTATUS(attr.setKeyable(false)); \
CHECK_MSTATUS(attr.setStorable(false)); \
CHECK_MSTATUS(attr.setReadable(true)); \
CHECK_MSTATUS(attr.setWritable(false));

#define MCheckErr(stat, msg) \
		if ( MS::kSuccess != stat ) { \
			cerr << msg; \
			return MS::kFailure; \
		}

ZoomorphNode::ZoomorphNode()
{
}


ZoomorphNode::~ZoomorphNode()
{
}

void printsomestuff(MDataHandle meshData) {
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
}

MStatus ZoomorphNode::compute(const MPlug & plug, MDataBlock & data)
{
	MStatus returnStatus;
	if (plug == outputMesh)
	{
		// getting baseMesh data??
		MDataHandle baseMeshData = data.inputValue(baseMesh, &returnStatus);
		MCheckErr(returnStatus, "ERROR getting base mesh data handle\n");
		MFnMesh mesh = baseMeshData.asMesh();

		// get animal mesh
		MDataHandle animalMeshData = data.inputValue(animalMesh, &returnStatus);
		MCheckErr(returnStatus, "ERROR getting animal mesh data handle\n");
		MFnMesh animMesh = animalMeshData.asMesh();

		// Get weight
		MDataHandle weightingData = data.inputValue(weighting, &returnStatus);
		MCheckErr(returnStatus, "ERROR getting time data handle\n");
		float weight = weightingData.asFloat();

        // Get weight
        MDataHandle gridData = data.inputValue(gridSize, &returnStatus);
        MCheckErr(returnStatus, "ERROR getting gridSize data handle\n");
        int gridDim = gridData.asInt();

		// get com weight
		MDataHandle comEnergyHandle = data.outputValue(comEnergy, &returnStatus);
		MCheckErr(returnStatus, "ERROR getting com data handle\n");

        // get output object
        MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
        MCheckErr(returnStatus, "ERROR getting output data handle\n");

        float com;

        // Create Zoomorph Mesh in our mesh structure
        Mesh* ZoomorphMesh = this->getZoomorphMesh(animalMeshData, baseMeshData, weight, gridDim, com);

        // ---------- Creating new maya mesh from our mesh structure ---------------

		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);

        createMesh(ZoomorphMesh, newOutputData, returnStatus);

        comEnergyHandle.set(com);
		outputHandle.set(newOutputData);
		data.setClean(plug);

		return MS::kSuccess;

	}
	else
	{
		return MS::kUnknownParameter;
	}
    return MS::kSuccess;
}

void * ZoomorphNode::creator()
{
    return new ZoomorphNode();
}

MStatus ZoomorphNode::initialize()
{
	MFnTypedAttribute typedAttributeFn;
	MFnNumericAttribute numAttr;

	MStatus returnStatus;

	// -------- Create attributes --------------
	
	// Animal mesh
	ZoomorphNode::animalMesh = typedAttributeFn.create("animalMesh", "animalM", MFnData::kMesh, &returnStatus);
	MCheckErr(returnStatus, "ERROR creating animal mesh attribute\n");
	MAKE_INPUT(typedAttributeFn);

	// Base mesh
	ZoomorphNode::baseMesh = typedAttributeFn.create("baseMesh", "baseM", MFnData::kMesh, &returnStatus);
	MCheckErr(returnStatus, "ERROR creating base mesh attribute\n");
	MAKE_INPUT(typedAttributeFn);

	// Weighting factor
	ZoomorphNode::weighting = numAttr.create("weighting", "w", MFnNumericData::kFloat, 0.5f, &returnStatus);
	MCheckErr(returnStatus, "ERROR creating weighting attribute\n");
	MAKE_INPUT(numAttr);

    // Grid Size
    ZoomorphNode::gridSize = numAttr.create("gridSize", "g", MFnNumericData::kInt, 20, &returnStatus);
    MCheckErr(returnStatus, "ERROR creating weighting attribute\n");
    MAKE_INPUT(numAttr);

	// Output mesh
	ZoomorphNode::outputMesh = typedAttributeFn.create("outputMesh", "outM", MFnData::kMesh, &returnStatus);
	MCheckErr(returnStatus, "ERROR creating outputMesh attribute\n");
	MAKE_OUTPUT(typedAttributeFn);

    // Output mesh
    ZoomorphNode::comEnergy = numAttr.create("comEnergy", "com", MFnNumericData::kFloat, 0.0f, &returnStatus);
    MCheckErr(returnStatus, "ERROR creating energy attribute\n");
    MAKE_OUTPUT(numAttr);

	// ------------- Add attributes ---------------

	// Animal mesh
	returnStatus = addAttribute(ZoomorphNode::animalMesh);
	MCheckErr(returnStatus, "ERROR adding animalmesh attribute\n");

	// Base mesh
	returnStatus = addAttribute(ZoomorphNode::baseMesh);
	MCheckErr(returnStatus, "ERROR adding basemesh attribute\n");

	// Weighting factor
	returnStatus = addAttribute(ZoomorphNode::weighting);
	MCheckErr(returnStatus, "Error adding weighting attribute\n");

    // Weighting factor
    returnStatus = addAttribute(ZoomorphNode::gridSize);
    MCheckErr(returnStatus, "Error adding weighting attribute\n");

	// Output mesh
	returnStatus = addAttribute(ZoomorphNode::outputMesh);
	MCheckErr(returnStatus, "ERROR adding outputmesh attribute\n");

    // Output mesh
    returnStatus = addAttribute(ZoomorphNode::comEnergy);
    MCheckErr(returnStatus, "ERROR adding energy attribute\n");

    return (MS::kSuccess);
}

// This function takes in the Mesh format defined in this project and creates a 
// MFnMesh structure and plugs that in to OutData so the Zoomorph mesh can be 
// rendered in Maya.
MObject ZoomorphNode::createMesh(Mesh* ZoomorphMesh, MObject& outData, MStatus& stat) {

    MStreamUtils::stdOutStream() << "CREATING ZOOMORPH MESH" << "\n";

    int numVertices = 0;
    int numPolygons = ZoomorphMesh->faces.size();
    MFloatPointArray vertexArray;
    MIntArray polygonCounts;
    MIntArray polygonConnects;

    for (int i = 0; i < ZoomorphMesh->faces.size(); i++) {
        Triangle* tri = ZoomorphMesh->faces[i];
        numVertices += 3;

        MFloatPoint vertex1(tri->points[0].x, tri->points[0].y, tri->points[0].z);
        MFloatPoint vertex2(tri->points[1].x, tri->points[1].y, tri->points[1].z);
        MFloatPoint vertex3(tri->points[2].x, tri->points[2].y, tri->points[2].z);
        vertexArray.append(vertex1);
        vertexArray.append(vertex2);
        vertexArray.append(vertex3);

        polygonCounts.append(3);
        polygonConnects.append(i * 3);
        polygonConnects.append(i * 3 + 2);
        polygonConnects.append(i * 3 + 1);

        MStreamUtils::stdOutStream() << tri->points[0].x << " , " <<  tri->points[0].y << " , " << tri->points[0].z << "\n";
        MStreamUtils::stdOutStream() << tri->points[1].x << " , " << tri->points[1].y << " , " << tri->points[1].z << "\n";
        MStreamUtils::stdOutStream() << tri->points[2].x << " , " << tri->points[2].y << " , " << tri->points[2].z << "\n";
    }

    MFnMesh meshFS;

    MStreamUtils::stdOutStream() << numVertices << "\n"; //36
    MStreamUtils::stdOutStream() << numPolygons << "\n"; // 12

    MObject newMesh = meshFS.create(numVertices, numPolygons,
        vertexArray, polygonCounts, polygonConnects,
        outData, &stat);

    return newMesh;
}

// Creates a cube mesh for testing purposes
MObject ZoomorphNode::createCubeMesh(MObject& outData, MStatus& stat) {	
	MFnMesh zoomorphMesh;
	MStatus status;

	// ---------------Referencing animCubeNode.cpp ------------------

	int numVertices, frame;
	float cubeSize;
	MFloatPointArray points;
	MFnMesh meshFS;

	cubeSize = 2.0f;

	const int numFaces = 6;
	numVertices = 8;
	const int numFaceConnects = 24;

	MFloatPoint vtx_1(-cubeSize, -cubeSize, -cubeSize);
	MFloatPoint vtx_2(cubeSize, -cubeSize, -cubeSize);
	MFloatPoint vtx_3(cubeSize, -cubeSize, cubeSize);
	MFloatPoint vtx_4(-cubeSize, -cubeSize, cubeSize);
	MFloatPoint vtx_5(-cubeSize, cubeSize, -cubeSize);
	MFloatPoint vtx_6(-cubeSize, cubeSize, cubeSize);
	MFloatPoint vtx_7(cubeSize, cubeSize, cubeSize);
	MFloatPoint vtx_8(cubeSize, cubeSize, -cubeSize);
	points.append(vtx_1);
	points.append(vtx_2);
	points.append(vtx_3);
	points.append(vtx_4);
	points.append(vtx_5);
	points.append(vtx_6);
	points.append(vtx_7);
	points.append(vtx_8);

	// Set up an array containing the number of vertices
	// for each of the 6 cube faces (4 vertices per face)
	//
	int face_counts[numFaces] = { 4, 4, 4, 4, 4, 4 };
	MIntArray faceCounts(face_counts, numFaces);

	// Set up and array to assign vertices from points to each face 
	//
	int face_connects[numFaceConnects] = { 0, 1, 2, 3,
											4, 5, 6, 7,
											3, 2, 6, 5,
											0, 3, 5, 4,
											0, 4, 7, 1,
											1, 7, 6, 2 };
	MIntArray faceConnects(face_connects, numFaceConnects);

	MObject newMesh = meshFS.create(numVertices, numFaces,
		points, faceCounts, faceConnects,
		outData, &stat);

	return newMesh;
}

Mesh* ZoomorphNode::getZoomorphMesh(MDataHandle animMeshHandle, MDataHandle baseMeshHandle, double w, int gridSize,
    float& com) {
    Mesh* animalMesh2 = new Mesh();
    Mesh* baseMesh2 = new Mesh();

    MStreamUtils::stdOutStream() << "ANIMAL MESH TO OUR MESH" << "\n";
    animalMesh2->CreateMeshFromMayaMesh(animMeshHandle);

    MStreamUtils::stdOutStream() << "BASE MESH TO OUR MESH" << "\n";
    baseMesh2->CreateMeshFromMayaMesh(baseMeshHandle);

    Grid* grid = new Grid(baseMesh2->faces, animalMesh2->faces, gridSize);
    grid->ComputeSourceSignedDistanceFunctions(baseMesh2);
    grid->ComputeTargetSignedDistanceFunctions(animalMesh2);

    int iterations = (int) w;
    
    for (int i = 0; i < iterations; i++) {
        grid->Update();
    }

    Mesh* zoomorphMesh = grid->BuildMesh();

    /*glm::vec3 animCOM = this->CenterOfMassEnergy(animalMesh2);
    glm::vec3 baseCOM = this->CenterOfMassEnergy(baseMesh2);

    com = glm::length(animCOM - baseCOM);*/

    return zoomorphMesh;
}
/*
glm::vec3 ZoomorphNode::CenterOfMassEnergy(Mesh* mesh_source) {
    glm::vec3 barycenter(0.f);
    float totalSA = 0.f;
    for (Triangle* f : mesh_source->faces) {
        glm::vec3 centroid_onTriangle = (f->points[0] + f->points[1] + f->points[2]) / 3.f;
        float surfaceArea_onTriangle = 0.5f * glm::length(glm::cross(f->points[0] - f->points[1], f->points[2] - f->points[1]));

        barycenter += centroid_onTriangle * surfaceArea_onTriangle;
        totalSA += surfaceArea_onTriangle;
    }
    barycenter /= totalSA;
    return barycenter;
}*/
