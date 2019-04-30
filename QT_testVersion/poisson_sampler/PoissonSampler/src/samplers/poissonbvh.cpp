#include "poissonsampler.h"

PoissonBVH::PoissonBVH(Mesh* m) : root(new P_BVHNode()) {
    buildBVH(m);
}

PoissonBVH::~PoissonBVH() {
    delete root;
}

GLenum PoissonBVH::drawMode() const {
    return GL_LINES;
}

void PoissonBVH::create() {
    // create every bbox for every node in this tree --> bbox's are the drawable items

    if (root != nullptr) {
        root->create();
        if (root->l != nullptr) {
            root->l->create();
        }
        if (root->r != nullptr) {
            root->r->create();
        }
    }
}

void PoissonBVH::drawAll(MyGL& context, ShaderProgram& s) {
    if (root != nullptr) {
        root->draw(context, s);
    }
}


/**
 * @brief PoissonBVH::buildBVH
 * @param m - mesh from which the BVH will be built
 */
void PoissonBVH::buildBVH(Mesh* m){
    int minNumOfTris = 20;

    // building the whole tree
    root->buildSelfAsChild(m->faces, minNumOfTris);
}


bool PoissonBVH::within(const glm::vec3& pos) {
    //      does our ray shooting in this dimension
    //      from each of the 8 corner points (3 dimensions)
    //      ever hit the mesh an odd number of times?
    //          true? inside of the mesh : outside of the mesh
   // int withinMesh = 0;
    //for (int dim = 0; dim < 3; ++dim) {
        glm::vec3 dir(0.f);
        dir[0] = 1.f;

        Ray isxRay(pos, dir);
        bool evenNumIntersections = false;
        int numIsx = 0;

        //glm::vec3 normal(0.f);
        std::vector<glm::vec3> normals = std::vector<glm::vec3>();
        std::vector<float> ts = std::vector<float>();
        if (root->intersect(isxRay, &numIsx, normals, ts)) {
            glm::vec3 closestNormal = normals[0];
            float closestT = ts[0];
            for (int i = 1; i < normals.size(); ++i) {
                if (ts[i] > 0 && ts[i] < closestT) {
                    closestT = ts[i];
                    closestNormal = normals[i];
                }
            }
            if (glm::dot(closestNormal, dir) > 0) {
                return true;
            }
        }
        return false;


        // for it to be within the mesh, must have odd num isx in all dimensions
        // removes the problem of shooting along the direction of a face leading
        // to floating point error
        //withinMesh += (!evenNumIntersections) ? 1 : 0;
    //}
   // return (withinMesh > 2);
/*
    // find all intersections - if normal of closest intersection is in other direction, return.
    bool withinMesh = true;
    glm::vec3 dir(1, 0, 0);
    Ray isxRay(pos, dir);
    int numIsx = 0;
    glm::vec3 normal(0.f);
    if (root->intersect(isxRay, &numIsx, normal)) {
        if (glm::dot(normal, dir) > 0) {
            return true;
        } else {
            std::cout << "  NUM ISX: " << numIsx << std::endl;
        }
    }
    return false;*/
}

/**
 * @brief PoissonBVH::intersect - traces down BVH tree to find node of intersection if it exists
 * @param ray - ray send from any direction towards object - finding intersection from here
 * @param isect - variable to filled in with intersect location if it exists; false, otherwise.
 * @return true if intersects any nodes in the BVH; false, otherwise.
 */
bool PoissonBVH::intersect(Ray &ray, int* numIntersections, std::vector<glm::vec3>& normals, std::vector<float>& t) {
    return root->intersect(ray, numIntersections, normals, t);
}

/**
 * @brief P_BVHNode::intersect
 * @param ray - ray send from any direction towards object - finding intersection from here
 * @param isect - variable to filled in with intersect location if it exists; false, otherwise.
 * @return true if intersects any nodes in the BVH; false, otherwise.
 */
bool P_BVHNode::intersect(const Ray &ray, int* numIntersections, std::vector<glm::vec3>& normals, std::vector<float>& ts) {

    if (l == nullptr && r == nullptr) {
        // hit leaf node

        bool hit = false;
        for (std::shared_ptr<Triangle> triangle: tris) {
            // return closest intersection

            if (triangle->Intersect_PBVH(ray, normals, ts)) {
                *numIntersections += 1;
                hit = true;
            }
        }
        return hit;

    } else if ((l == nullptr && r != nullptr) || (l!= nullptr && r == nullptr))
        { std::cout<<"error in P_BVHNode intersect childNodes not proper [ie have exactly one child node]"<<std::endl; throw; }

    bool l_hasIsx = l->bbox->Intersect(ray, nullptr);
    bool r_hasIsx = r->bbox->Intersect(ray, nullptr);
    if (!l_hasIsx && !r_hasIsx) {
        return false;
    }

    // checking none or single case of box intersection
    int numIsxL = 0;
    int numIsxR = 0;
    l_hasIsx = l->intersect(ray, &numIsxL, normals, ts);
    r_hasIsx = r->intersect(ray, &numIsxR, normals, ts);

    // checking for recursive bbox-to-triangle intersections
    *numIntersections = numIsxL + numIsxR;

    return *numIntersections > 0;
}

