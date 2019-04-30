#include "bounds.h"

Bounds3f Union(const Bounds3f& b1, const Bounds3f& b2) {
    if (b1.min.x > b1.max.x) { return b2; }
    if (b2.min.x > b2.max.x) { return b1; }
    return Bounds3f(Point3f(std::min(b1.min.x, b2.min.x),
                            std::min(b1.min.y, b2.min.y),
                            std::min(b1.min.z, b2.min.z)),
                    Point3f(std::max(b1.max.x, b2.max.x),
                            std::max(b1.max.y, b2.max.y),
                            std::max(b1.max.z, b2.max.z)));
}

Bounds3f Union(const Bounds3f& b1, const glm::vec3& p) {
    if (b1.min.x > b1.max.x) { return Bounds3f(p); }
    return Bounds3f(Point3f(std::min(b1.min.x, p.x),
                            std::min(b1.min.y, p.y),
                            std::min(b1.min.z, p.z)),
                    Point3f(std::max(b1.max.x, p.x),
                            std::max(b1.max.y, p.y),
                            std::max(b1.max.z, p.z)));
}

Bounds3f* BuildBoundingBox(const std::vector<Triangle*>& t) {
    Bounds3f* b = nullptr;
    for (Triangle* tri: t) {
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

Bounds3f::Bounds3f() : min(INFINITY), max(-INFINITY) {}
Bounds3f::Bounds3f(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
Bounds3f::Bounds3f(const glm::vec3& p) : min(p), max(p) {}
Bounds3f::Bounds3f(const Bounds3f& b) : min(b.min), max(b.max) {}

Bounds3f::Bounds3f(const std::vector<Triangle*>& t) {
    *this = *(BuildBoundingBox(t));
}

Bounds3f::~Bounds3f() {}

 bool Bounds3f::Intersect(const glm::vec3& origin, const glm::vec3& direction, float* t) const
{
    if (t == nullptr) { t = new float(); }
    if (min.x > max.x) return false;

    // do intersection based on cube's intersection function
    glm::vec3 ray_origin(origin);
    glm::vec3 ray_direction(direction);

    float t_n = -INFINITY;
    float t_f = INFINITY;
    for(int i = 0; i < 3; i++) {
        //Ray parallel to slab check
        if(ray_direction[i] == 0){
            // if parrallel then the ray origin must be within for the following values
            //      so if not then false
            if(ray_origin[i] < min[i] || ray_origin[i] > max[i]){
                return false;
            }
        }

        //If not parallel, do slab intersect check
        float t0 = (min[i] - ray_origin[i])/ray_direction[i];
        float t1 = (max[i] - ray_origin[i])/ray_direction[i];

        // set tnear and tfar to proper vals
        if(t0 > t1){
            float temp = t1;
            t1 = t0;
            t0 = temp;
        }
        if(t0 > t_n){
            t_n = t0;
        }
        if(t1 < t_f){
            t_f = t1;
        }
    }

    if(t_n <= t_f)
    {
        // assign t to proper value
//        *t = t_n > 0 ? t_n : t_f;
        bool within =  (ray_origin.x <= max.x && ray_origin.x >= min.x
                        && ray_origin.y <= max.y && ray_origin.y >= min.y
                        && ray_origin.z <= max.z && ray_origin.y >= min.z);

        if ((t_n < 0 && within) || t_n > 0) {
            *t = t_n;
        } else {
            *t = t_f;
        }

        // if t is neg - this is only valid isect iff the ray's origin is within the bb
        bool returnVal = ((*t < 0 && within) || *t > 0);
        return returnVal;
    }
    //If t_near was greater than t_far, we did not hit the cube
    return false;
}


//GLenum Bounds3f::drawMode() const {
//    return GL_LINES;
//}

//void Bounds3f::create() {

//    std::vector<glm::vec3> vert_pos;
//    std::vector<glm::vec3> vert_nor;
//    std::vector<glm::vec3> vert_col;
//    std::vector<GLuint> vert_idx;

//    // setting up vertex locations

//    glm::vec3 min = bbox->min;
//    glm::vec3 max = bbox->max;

//    // 1    0 max
//    // 3    2
//    //
//    // 5    4
//    // 7    6
//    // min

//    vert_pos.push_back(glm::vec3(max[0], max[1], max[2])); // 0
//    vert_pos.push_back(glm::vec3(min[0], max[1], max[2])); // 1
//    vert_pos.push_back(glm::vec3(max[0], max[1], min[2])); // 2
//    vert_pos.push_back(glm::vec3(min[0], max[1], min[2])); // 3
//    vert_pos.push_back(glm::vec3(max[0], min[1], max[2])); // 4
//    vert_pos.push_back(glm::vec3(min[0], min[1], max[2])); // 5
//    vert_pos.push_back(glm::vec3(max[0], min[1], min[2])); // 6
//    vert_pos.push_back(glm::vec3(min[0], min[1], min[2])); // 7

//    vert_idx.push_back(2); // top
//    vert_idx.push_back(0);
//    vert_idx.push_back(0);
//    vert_idx.push_back(1);
//    vert_idx.push_back(1);
//    vert_idx.push_back(3);
//    vert_idx.push_back(3);
//    vert_idx.push_back(2);

//    vert_idx.push_back(2); // right
//    vert_idx.push_back(6);
//    vert_idx.push_back(6);
//    vert_idx.push_back(4);
//    vert_idx.push_back(4);
//    vert_idx.push_back(0);
//    vert_idx.push_back(0);
//    vert_idx.push_back(2);

//    vert_idx.push_back(2); // front
//    vert_idx.push_back(3);
//    vert_idx.push_back(3);
//    vert_idx.push_back(7);
//    vert_idx.push_back(7);
//    vert_idx.push_back(6);
//    vert_idx.push_back(6);
//    vert_idx.push_back(2);

//    vert_idx.push_back(2);
//    vert_idx.push_back(6);

//    vert_idx.push_back(6);
//    vert_idx.push_back(7);
//    vert_idx.push_back(7);
//    vert_idx.push_back(5); // bottom
//    vert_idx.push_back(5);
//    vert_idx.push_back(4);
//    vert_idx.push_back(4);
//    vert_idx.push_back(6);

//    vert_idx.push_back(6);
//    vert_idx.push_back(7);

//    vert_idx.push_back(7);
//    vert_idx.push_back(3); // left
//    vert_idx.push_back(3);
//    vert_idx.push_back(1);
//    vert_idx.push_back(1);
//    vert_idx.push_back(5);

//    vert_idx.push_back(1);  // back
//    vert_idx.push_back(0);
//    vert_idx.push_back(0);
//    vert_idx.push_back(4);
//    vert_idx.push_back(4);
//    vert_idx.push_back(5);

//    for(int i = 0; i < vert_idx.size(); i++){

//        vert_col.push_back(glm::vec3(1, 1, 1));
//        vert_nor.push_back(glm::vec3(0, 0, 1));
//    }

//    count = vert_idx.size();
//    int vert_count = vert_pos.size();

//    bufIdx.create();
//    bufIdx.bind();
//    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    bufIdx.allocate(vert_idx.data(), count * sizeof(GLuint));

//    bufPos.create();
//    bufPos.bind();
//    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    bufPos.allocate(vert_pos.data(), vert_count * sizeof(glm::vec3));

//    bufCol.create();
//    bufCol.bind();
//    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    bufCol.allocate(vert_col.data(), vert_count * sizeof(glm::vec3));

//    bufNor.create();
//    bufNor.bind();
//    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    bufNor.allocate(vert_nor.data(), vert_count * sizeof(glm::vec3));
//}

