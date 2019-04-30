#include "poissonbvhnode.h"


P_BVHNode::P_BVHNode() : l(nullptr), r(nullptr), tris(QList<std::shared_ptr<Triangle>>()), bbox(nullptr) {}
P_BVHNode::P_BVHNode(P_BVHNode* left, P_BVHNode* right)
    : l(left), r(right), tris(QList<std::shared_ptr<Triangle>>()), bbox(nullptr) {}

P_BVHNode::~P_BVHNode() {}

GLenum P_BVHNode::drawMode() const {
    return GL_LINES;
}

void P_BVHNode::create() {

    std::vector<glm::vec3> vert_pos;
    std::vector<glm::vec3> vert_nor;
    std::vector<glm::vec3> vert_col;
    std::vector<GLuint> vert_idx;

    // setting up vertex locations

    glm::vec3 min = bbox->min;
    glm::vec3 max = bbox->max;

    // 1    0 max
    // 3    2
    //
    // 5    4
    // 7    6
    // min

    vert_pos.push_back(glm::vec3(max[0], max[1], max[2])); // 0
    vert_pos.push_back(glm::vec3(min[0], max[1], max[2])); // 1
    vert_pos.push_back(glm::vec3(max[0], max[1], min[2])); // 2
    vert_pos.push_back(glm::vec3(min[0], max[1], min[2])); // 3
    vert_pos.push_back(glm::vec3(max[0], min[1], max[2])); // 4
    vert_pos.push_back(glm::vec3(min[0], min[1], max[2])); // 5
    vert_pos.push_back(glm::vec3(max[0], min[1], min[2])); // 6
    vert_pos.push_back(glm::vec3(min[0], min[1], min[2])); // 7

    vert_idx.push_back(2); // top
    vert_idx.push_back(0);
    vert_idx.push_back(0);
    vert_idx.push_back(1);
    vert_idx.push_back(1);
    vert_idx.push_back(3);
    vert_idx.push_back(3);
    vert_idx.push_back(2);

    vert_idx.push_back(2); // right
    vert_idx.push_back(6);
    vert_idx.push_back(6);
    vert_idx.push_back(4);
    vert_idx.push_back(4);
    vert_idx.push_back(0);
    vert_idx.push_back(0);
    vert_idx.push_back(2);

    vert_idx.push_back(2); // front
    vert_idx.push_back(3);
    vert_idx.push_back(3);
    vert_idx.push_back(7);
    vert_idx.push_back(7);
    vert_idx.push_back(6);
    vert_idx.push_back(6);
    vert_idx.push_back(2);

    vert_idx.push_back(2);
    vert_idx.push_back(6);

    vert_idx.push_back(6);
    vert_idx.push_back(7);
    vert_idx.push_back(7);
    vert_idx.push_back(5); // bottom
    vert_idx.push_back(5);
    vert_idx.push_back(4);
    vert_idx.push_back(4);
    vert_idx.push_back(6);

    vert_idx.push_back(6);
    vert_idx.push_back(7);

    vert_idx.push_back(7);
    vert_idx.push_back(3); // left
    vert_idx.push_back(3);
    vert_idx.push_back(1);
    vert_idx.push_back(1);
    vert_idx.push_back(5);

    vert_idx.push_back(1);  // back
    vert_idx.push_back(0);
    vert_idx.push_back(0);
    vert_idx.push_back(4);
    vert_idx.push_back(4);
    vert_idx.push_back(5);

    for(unsigned int i = 0; i < vert_idx.size(); i++){
        vert_col.push_back(glm::vec3(1, 1, 1));
        vert_nor.push_back(glm::vec3(0, 0, 1));
    }

    count = vert_idx.size();
    int vert_count = vert_pos.size();

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(vert_idx.data(), count * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(vert_pos.data(), vert_count * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(vert_col.data(), vert_count * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(vert_nor.data(), vert_count * sizeof(glm::vec3));


    if (l != nullptr) {
        l->create();
    }
    if (r != nullptr) {
        r->create();
    }
}


void P_BVHNode::draw(MyGL& context, ShaderProgram& s) {
    s.draw(context, *this);
    if (this->l != nullptr) {
        l->draw(context, s);
    }
    if (this->r != nullptr) {
        r->draw(context, s);
    }
}


/**
 * @brief P_BVHNode::buildSelfAsChild
 * @param t - the list of triangles to possibly be added to this node
 * @param minNum - min num of tris allowed for this node
 */
void P_BVHNode::buildSelfAsChild(QList<std::shared_ptr<Triangle>>& t, int minNum) {

    // notes:
    // if current object has fewer than minNum objects then this is a leaf node
    // do the split along the longest axis
    // use surface area heuristic to determine which should be in left and which should be in right
    // build left and right children
    // done

    this->bbox = this->buildBoundingBox(t);

    // base case
    if (t.length() < minNum) {
        // leaf node
        this->tris = t;

        // checking for construction errors
        if (this->bbox == nullptr) { std::cout<<"error in P_BVHNode buildSelfASChild"<<std::endl; throw; }
        if (this->l != nullptr) { std::cout<<"error in P_BVHNode buildSelfASChild"<<std::endl; throw; }
        if (this->r != nullptr) { std::cout<<"error in P_BVHNode buildSelfASChild"<<std::endl; throw; }

        return;
    }

    // finding longest axis
    glm::vec3 diag = this->bbox->Diagonal();
    float longest = glm::max(diag[0], glm::max(diag[1], diag[2]));
    int longestAxis = (longest == diag[0]) ? 0 : (longest == diag[1] ? 1 : 2);

    QList<std::shared_ptr<Triangle>>* leftList = new QList<std::shared_ptr<Triangle>>();
    QList<std::shared_ptr<Triangle>>* rightList = new QList<std::shared_ptr<Triangle>>();
    Bounds3f* leftBox = new Bounds3f();
    Bounds3f* rightBox = new Bounds3f();

    float SA = bbox->SurfaceArea();
    splitTheTris(longestAxis, t, leftList, rightList, bbox->min, bbox->max, leftBox, rightBox, SA);

    if (leftList->length() == 0 || rightList->length() == 0) { std::cout<<"error in P_BVHNode buildSelfASChild, lists not filled in correctly in splitTheTris"<<std::endl; throw; }

    this->l = new P_BVHNode();
    this->r = new P_BVHNode();

    this->l->buildSelfAsChild(*leftList, minNum);
    this->r->buildSelfAsChild(*rightList, minNum);
}

/**
 * @brief P_BVHNode::buildBoundingBox
 * @param t - list of triangles this bbox should encompass
 * @return pointer to bounding box encompassing all triangles in the inputted list
 */
Bounds3f* P_BVHNode::buildBoundingBox(QList<std::shared_ptr<Triangle>>& t) {
    Bounds3f* b = nullptr;
    for (std::shared_ptr<Triangle> tri: t) {
        Point3f minOfTri(0.0f);
        Point3f maxOfTri(0.0f);

        for (int i=0; i<3; i++) {
            minOfTri[i] = glm::min(tri->points[0][i], glm::min(tri->points[1][i], tri->points[2][i]));
            maxOfTri[i] = glm::max(tri->points[0][i], glm::max(tri->points[1][i], tri->points[2][i]));
        }

        if (b == nullptr) {
            b = new Bounds3f(minOfTri, maxOfTri);
        } else {
            *b = Union(*b, Union(minOfTri, maxOfTri));
        }
    }
    return b;
}

/**
 * @brief P_BVHNode::splitTheTris - split based on Surface Area Heuristic and using centroids to check the triangles bins/locations
 * @param axis - longest axis of bbox over which the triangles will be sorted/split
 * @param t - list of triangles to be split
 * @param left - the QList of triangles on one side of the discovered best split location
 * @param right - the QList of triangles on the other side of the discovered best split location
 * @param leftBox - bounding box for left [filled in this method]
 * @param rightBox - bounding box for right [filled in this method]
 */
void P_BVHNode::splitTheTris(int axis, QList<std::shared_ptr<Triangle>> &t,
                             QList<std::shared_ptr<Triangle>>* &left, QList<std::shared_ptr<Triangle>>* &right,
                             Point3f min, Point3f max,
                             Bounds3f* leftBox, Bounds3f* rightBox,
                             float outerSA) {

    // notes: doing binning on the longest axis based on centroid location of triangle and sah

    // check bbox min and bbox max -> for initialization if actually created properly
    for (int i=0; i<3; i++) {
        if (bbox->min[i] > bbox->max[i])
            { std::cout<<"error in P_BVHNode splitTheTris regarding bbox being created improperly for axis:"<<i<<std::endl; throw; }
    }

    // setting up vars for all looping iterations
    float stepSize = glm::vec3((max-min)/(50.0f))[axis];
    float loc = min[axis] + stepSize;
    float minSAH = std::numeric_limits<float>::max();

    bool neverSplit = true;

    // looping through vars at diff test locations
    while (loc < max[axis]) {

        // create this iter's variables
        QList<std::shared_ptr<Triangle>>* temp_tris_left = new QList<std::shared_ptr<Triangle>>();
        QList<std::shared_ptr<Triangle>>* temp_tris_right = new QList<std::shared_ptr<Triangle>>();
        Bounds3f* temp_box_left = new Bounds3f();
        Bounds3f* temp_box_right = new Bounds3f();
        float rightCount = 0.0f;
        float leftCount = 0.0f;

        // sort across curr loc
        for (std::shared_ptr<Triangle> tri: t) {
            Point3f tri_points[3] = tri.get()->points;
            Point3f centroid = (tri_points[0] + tri_points[1] + tri_points[2])/3.0f;
            if (centroid[axis] < loc) {
                leftCount += 1.0f;
                temp_tris_left->append(tri);
                temp_box_left = Union(temp_box_left, centroid);
            } else {
                rightCount += 1.0f;
                temp_tris_right->append(tri);
                temp_box_right = Union(temp_box_right, centroid);
            }
        }

        // test if curr split is good or not
        float rightSA = temp_box_right->SurfaceArea();
        float leftSA = temp_box_left->SurfaceArea();
        float SAdiff = glm::abs(rightSA * rightCount + leftSA * leftCount) / outerSA;

        if (SAdiff < minSAH) {
            neverSplit = false;

            minSAH = SAdiff;
            left->clear();
            right->clear();
            *left = *temp_tris_left;
            *right = *temp_tris_right;
            *leftBox = *temp_box_left;
            *rightBox = *temp_box_right;
        }

       // iterate to next checking location
        loc += stepSize;

    } //end: while (loc < max[axis]);

    /*if (neverSplit) {
        std::cout<<std::endl;
    }*/


    for (int i=0; i<3; i++) {
        if (leftBox->min[i] > leftBox->max[i])
            { std::cout<<"error in P_BVHNode splitTheTris leftBox never filled in for axis:"<<i<<std::endl; throw; }
        if (rightBox->min[i] > rightBox->max[i])
            { std::cout<<"error in P_BVHNode splitTheTris rightBox never filled in for axis:"<<i<<std::endl; throw; }
    }

    // done.
}

