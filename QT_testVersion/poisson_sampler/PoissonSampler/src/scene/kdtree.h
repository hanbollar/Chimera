#pragma once

#include "globals.h"
#include "triangle.h"

class KDNode {
private:
    KDNode* left_;
    KDNode* right_;

    unsigned int axis_;

    const unsigned int MAX_NUM_ITEMS_AT_LEAF = 1;

    glm::vec3 min_bound_ = glm::vec3(INFINITY);
    glm::vec3 max_bound_ = glm::vec3(-INFINITY);
protected:
    std::vector<Triangle*> tris_;
public:
    KDNode();
    KDNode(const std::vector<Triangle*>& tris);
    KDNode(const std::vector<Triangle*>& tris, const glm::vec3& min_corner, const glm::vec3& max_corner);
    ~KDNode();

    void BuildWithTriangles(const std::vector<Triangle*>& tris);

    bool Within(const glm::vec3& loc, int& isx_count, const glm::vec3& dir);
    bool IsLeaf();
    float GetClosestDistanceToPoint(Point3f p);
    void GetNearestPoint(KDNode node, Point3f q);
};

class KDTree {
private:
    KDNode* root_ = nullptr;
public:
    KDTree();
    KDTree(const std::vector<Triangle*>& tris);
    ~KDTree();

    void BuildWithTriangles(const std::vector<Triangle*>& tris);
    bool WithinAnyShape(const glm::vec3& loc);
    void GetNearestPoint(Point3f q);
};
