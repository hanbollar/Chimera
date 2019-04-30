#include "kdtree.h"

// Global variable
Point3f bestDistancePoint;
float bestDistance = INFINITY;

KDTree::KDTree() : root_(nullptr) {}

KDTree::KDTree(const std::vector<Triangle*>& tris) : root_(new KDNode(tris))
{}

KDTree::~KDTree() {
    delete root_;
}

bool KDTree::WithinAnyShape(const glm::vec3& loc) {
    // just testing one position, not testing whole user_character_figure_
    // not optimal for implementation but rudimentary enough to work for now
    // TODO: optimize this better

    int isx_count = 0;
    root_->Within(loc, isx_count);
    return (isx_count % 2 != 0); // not even, then *is* within the shape
}

void KDTree::BuildWithTriangles(const std::vector<Triangle*>& tris) {
    if (root_ == nullptr) {
        root_ = new KDNode(tris);
    } else {
        delete root_;
        root_ = new KDNode(tris);
    }
}

void KDTree::GetNearestPoint(Point3f Q) {
    root_->GetNearestPoint(root_, Q);
}

/**************/

KDNode::KDNode() : min_bound_(glm::vec3(0)), max_bound_ (glm::vec3(0)), left_(nullptr), right_(nullptr) {}

KDNode::KDNode(const std::vector<Triangle*>& tris) : left_(nullptr), right_(nullptr) {
    for (Triangle* t : tris) {
        for (int i = 0; i < 3; ++i) { // point
            for (int j = 0; j < 3; ++j) { // axis
                min_bound_[j] = glm::min(min_bound_[j], t->points[i][j]);
                max_bound_[j] = glm::max(max_bound_[j], t->points[i][j]);
            }
        }
    }

    if (tris.size() == MAX_NUM_ITEMS_AT_LEAF) {
        tris_ = tris;
    } else {
        BuildWithTriangles(tris);
    }
}

KDNode::KDNode(const std::vector<Triangle*>& tris, const glm::vec3& min_corner, const glm::vec3& max_corner)
    : min_bound_(min_corner), max_bound_(max_corner) {

    // invalid setup check
    for (int i = 0; i < 3; ++i) {
        if (min_corner[i] > max_corner[i]) {
            std::cout <<"Invalid min, max corner setup for KDNode"<<std::endl;
            throw;
        }
    }

    if (tris.size() == MAX_NUM_ITEMS_AT_LEAF) {
        tris_ = tris;
    } else {
        BuildWithTriangles(tris);
    }
}

KDNode::~KDNode() {
    tris_.clear();
    delete left_;
    delete right_;
}

void KDNode::BuildWithTriangles(const std::vector<Triangle*>& tris) {
    // already guaranted that this is false: (tris.size() <= MAX_NUM_ITEMS_AT_LEAF)

    // split longest axis in half
    if (max_bound_.x < min_bound_.x || max_bound_.y < min_bound_.y || max_bound_.z < min_bound_.z) {
        std::cout<<"Improper configuration in KDNode."<<std::endl;
        throw;
    }

    glm::vec3 diff = glm::abs(max_bound_ - min_bound_);
    axis_ = (diff.x > diff.y && diff.x > diff.z) ? 0 : (diff.y > diff.x && diff.y > diff.z) ? 1 : 2;

    int split_index = tris.size() / 2.f;

    std::vector<Triangle*> left_tris(tris.begin(), tris.begin() + split_index);
    std::vector<Triangle*> right_tris(tris.begin() + split_index, tris.end());

    if (left_tris.size() > 0) {
        left_ = new KDNode(left_tris);
    }
    if (right_tris.size() > 0) {
        right_ = new KDNode(right_tris);
    }
}

bool CubeIntersect(const glm::vec3& origin, const glm::vec3& direction) {
    //Transform the ray
    //Ray r_loc = r.GetTransformedCopy(transform.invT());

    float t_n = -INFINITY;
    float t_f = INFINITY;
    for(int i = 0; i < 3; i++){
        //Ray parallel to slab check
        if(direction[i] == 0){
            if(origin[i] < -0.5f || origin[i] > 0.5f){
                return false;
            }
        }
        //If not parallel, do slab intersect check
        float t0 = (-0.5f - origin[i])/direction[i];
        float t1 = (0.5f - origin[i])/direction[i];
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
    if(t_n < t_f)
    {
        float t = t_n > 0 ? t_n : t_f;
        if(t < 0)
            return false;
        //Lastly, transform the point found in object space by T
//        glm::vec4 P = glm::vec4(r_loc.origin + t*r_loc.direction, 1);
//        InitializeIntersection(isect, t, Point3f(P));
        return true;
    } else{
        //If t_near was greater than t_far, we did not hit the cube
        return false;
    }
}

bool KDNode::Within(const glm::vec3& loc, int& isx_count) {
    if (IsLeaf()) {
        for (Triangle* t: tris_) {
            // TODO: MIGHT NEED TO ADD A CONVERT TO TRI SPACE MATRIX DEPENDING ON HOW MAYA HANDLES ITS ISX AND OBJ CONFIGS FOR VERTICES
            if (t->Intersect(loc, SHOOTING_DIR)) {
                isx_count += 1;
                return true;
            }
        }
        return false;
    }

    // kd tree is axis aligned so to transform to cube space just need to do transl & scaling
    // to match unit cube centered at origin
    glm::vec3 ave_loc = (max_bound_ - min_bound_) * .5f;
    glm::mat4 transf_m_for_bounds = glm::translate(glm::scale(glm::mat4(1.f), max_bound_ - min_bound_), ave_loc);
    glm::mat4 inv_transp = glm::inverse(glm::transpose(transf_m_for_bounds));

    if (!CubeIntersect(glm::vec3(inv_transp * glm::vec4(loc, 1)), glm::vec3(inv_transp * glm::vec4(SHOOTING_DIR, 0)))) {
        return false;
    }

    left_->Within(loc, isx_count);
    right_->Within(loc, isx_count);

    return (isx_count % 2 != 0);
}

bool KDNode::IsLeaf() {
    return tris_.size() != 0;
}

float KDNode::GetClosestDistanceToPoint(Point3f p) {
      float dx = glm::max(glm::max(min_bound_.x - p.x, 0.0f), p.x - max_bound_.x);
      float dy = glm::max(glm::max(min_bound_.y - p.y, 0.0f), p.y - max_bound_.y);
      float dz = glm::max(glm::max(min_bound_.z - p.z, 0.0f), p.z - max_bound_.z);
      return glm::sqrt(dx*dx + dy*dy + dz * dz);
}

void KDNode::GetNearestPoint(KDNode node, Point3f p) {
    if (node == NULL || node.GetClosestDistanceToPoint(p) > bestDistance) {
        return;
    }
    if (node.IsLeaf()) {
        Point3f leafPoint = node.tris_.at(0)->GetClosestPointToAPoint(p);
        float dist = glm::distance(leafPoint, p);
        if (dist < bestDistance) {
            bestDistance = dist;
            bestDistancePoint = leafPoint;
        }
    }
    if (p[axis_] < node.min_bound_[axis_]) {
        GetNearestPoint(node.left_, p);
        GetNearestPoint(node.right_, p);
    } else {
        GetNearestPoint(node.left_, p);
        GetNearestPoint(node.right_, p);
    }
}
