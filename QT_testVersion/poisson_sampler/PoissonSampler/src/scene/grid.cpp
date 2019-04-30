#include "grid.h"

Grid::Grid()
    : Drawable(),
      bbox_(Bounds3f()),
      dim_of_cell_(glm::vec3(-1)),
      grid_cells_info_(std::vector<Vertex>()),
      grid_cells_interp_(std::vector<float>())
{}

Grid::Grid(const Bounds3f &bbox)
    : Drawable(),
      bbox_(bbox), dim_of_cell_(glm::vec3(0.f)),
      grid_cells_info_(std::vector<Vertex>(dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z, Vertex())),
      grid_cells_interp_(std::vector<float>(dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z, 0))
{
    dim_of_cell_ = (2.f*OFFSET + bbox.max - bbox.min) / glm::vec3(dim_num_cells_);
    origin_ = bbox.min - OFFSET;
}

Grid::Grid(const std::vector<Triangle*>& source, const std::vector<Triangle*>& target)
    : Drawable(),
      bbox_(Bounds3f::Union(*Bounds3f::BuildBoundingBox(source), *Bounds3f::BuildBoundingBox(target))), dim_of_cell_(glm::vec3(0.f)),
      grid_cells_info_(std::vector<Vertex>(dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z, Vertex())),
      grid_cells_interp_(std::vector<float>(dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z, 0))
{
    dim_of_cell_ = (2.f*OFFSET + bbox_.max - bbox_.min) / glm::vec3(dim_num_cells_);
    origin_ = bbox_.min - OFFSET;
}


int Grid::Convert3DCellIndexTo1DCellIndex(const int& i, const int& j, const int& k) {
    return i + j * dim_num_cells_.x + k * dim_num_cells_.x * dim_num_cells_.y;
}

glm::ivec3 Grid::Convert1DCellIndexTo3DCellIndex(const int& index) {
    return glm::ivec3(index % dim_num_cells_.x,
                      (index / dim_num_cells_.x) % dim_num_cells_.y,
                      index / (dim_num_cells_.x * dim_num_cells_.y));
}

glm::vec3 Grid::Convert3DCellIndexToLocation(const int& i, const int& j, const int& k) {
    return origin_ + glm::vec3(i * dim_of_cell_.x, j * dim_of_cell_.y, k * dim_of_cell_.z);
}

glm::vec3 Grid::Convert3DCellIndexToLocation(const glm::ivec3 indices) {
    return Convert3DCellIndexToLocation(indices[0], indices[1], indices[2]);
}

glm::vec3 Grid::Convert1DCellIndexToLocation(const int& i) {
    return Convert3DCellIndexToLocation(Convert1DCellIndexTo3DCellIndex(i));
}

glm::ivec3 Grid::ConvertLocationToCellIndex(const glm::vec3& loc) {
    return glm::ivec3((loc - origin_) / glm::vec3(dim_of_cell_));
}

void Grid::Update() {
    for (int i = 0; i < dim_num_cells_.x; ++i) {
        for (int j = 0; j < dim_num_cells_.y; ++j) {
            for (int k = 0; k < dim_num_cells_.z; ++k) {
                glm::ivec3 NEG_offset(-1);
                glm::ivec3 POS_offset(1);

                // edge cases for x
                if (i == 0) {
                    NEG_offset.x = 0;
                } else if (i == dim_num_cells_.x - 1) {
                    POS_offset.x = 0;
                }

                // edge cases for y
                if (j == 0) {
                    NEG_offset.y = 0;
                } else if (j == dim_num_cells_.y - 1) {
                    POS_offset.y = 0;
                }

                // edge cases for z
                if (k == 0) {
                    NEG_offset.z = 0;
                } else if (k == dim_num_cells_.z - 1) {
                    POS_offset.z = 0;
                }

                // trilinear interp
                // cube corner check - 8 vals
                float back_val0 = grid_cells_info_[Convert3DCellIndexTo1DCellIndex(i + NEG_offset.x, j + NEG_offset.y, k + POS_offset.z)].target_value;
                float back_val1 = grid_cells_info_[Convert3DCellIndexTo1DCellIndex(i + POS_offset.x, j + NEG_offset.y, k + POS_offset.z)].target_value;
                float back_val2 = grid_cells_info_[Convert3DCellIndexTo1DCellIndex(i + NEG_offset.x, j + POS_offset.y, k + POS_offset.z)].target_value;
                float back_val3 = grid_cells_info_[Convert3DCellIndexTo1DCellIndex(i + POS_offset.x, j + POS_offset.y, k + POS_offset.z)].target_value;

                float front_val0 = grid_cells_info_[Convert3DCellIndexTo1DCellIndex(i + NEG_offset.x, j + NEG_offset.y, k + NEG_offset.z)].target_value;
                float front_val1 = grid_cells_info_[Convert3DCellIndexTo1DCellIndex(i + POS_offset.x, j + NEG_offset.y, k + NEG_offset.z)].target_value;
                float front_val2 = grid_cells_info_[Convert3DCellIndexTo1DCellIndex(i + NEG_offset.x, j + POS_offset.y, k + NEG_offset.z)].target_value;
                float front_val3 = grid_cells_info_[Convert3DCellIndexTo1DCellIndex(i + POS_offset.x, j + POS_offset.y, k + NEG_offset.z)].target_value;
                // lerp along x - 4 vals
                float back_bot_lerpx = (back_val0 + back_val1) / 2.f;
                float back_top_lerpx = (back_val2 + back_val3) / 2.f;

                float front_bot_lerpx = (front_val0 + front_val1) / 2.f;
                float front_top_lerpx = (front_val2 + front_val3) /2.f;
                // lerp along y - 2 vals
                float front_mid_lerpy = (front_bot_lerpx + front_top_lerpx)/2.f;

                float back_mid_lerpy = (back_bot_lerpx + back_top_lerpx) / 2.f;
                // lerp along z - 1 val
                float tri_lerp = (front_mid_lerpy + back_mid_lerpy) / 2.f;
                // fill in
                grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(i, j, k)] += tri_lerp;
            }
        }
    }
}

float ClosestDistanceToMesh(const glm::vec3& loc, const Mesh* m) {
    Point3f closest = m->tree->CalculateNearestPoint(loc);
    return glm::distance(closest, loc);
}

bool BadWithinTest(const glm::vec3& loc, const Mesh* m) {
    int num_isx = 0;

    bool within = true;
    int num_within = 0;
    for (int i = 0; i < 3; ++i) {
        num_isx = 0;
        glm::vec3 dir = (i == 0) ? glm::vec3(1, 0, 0) : (i == 1) ? glm::vec3(0, 1, 0) : glm::vec3(0, 0, 1);
        for (Triangle* f : m->faces) {
            num_isx += (f->Intersect(loc, dir)) ? 1 : 0;
        }
        num_within += (num_isx % 2 != 0) ? 1 : 0;
        within &= (num_isx % 2 != 0);
    }
    return num_within >= 2;
}

void Grid::ComputeSignedDistanceFunctions(const Mesh* m, const bool& source) {
    float tot_num_cells = dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z;
    for (int i = 0; i < tot_num_cells; ++i) {
        std::cout << "on cell: " << i << " of " << tot_num_cells << std::endl;
        glm::vec3 grid_loc_position = Convert1DCellIndexToLocation(i);

        // inside vs outside
        float sign = BadWithinTest(grid_loc_position, m) /*m->tree->WithinAnyShape(grid_loc_position)*/ ? 1 : -1;

        // actual sdf
        if (source) {
            grid_cells_info_[i].source_value = sign * ClosestDistanceToMesh(grid_loc_position, m);
            grid_cells_interp_[i] = grid_cells_info_[i].source_value;
        } else {
            grid_cells_info_[i].target_value = sign * ClosestDistanceToMesh(grid_loc_position, m);
        }
    }
}

void Grid::ComputeSourceSignedDistanceFunctions(const Mesh* m_source) {
    ComputeSignedDistanceFunctions(m_source, true);
}

void Grid::ComputeTargetSignedDistanceFunctions(const Mesh* m_target) {
    ComputeSignedDistanceFunctions(m_target, false);
}

GLenum Grid::drawMode() const {
    return GL_POINTS;
}

void Grid::create() {
    create(2);
}


void Grid::create(int view_src_trg_lerp) {
    count = dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z;

    std::vector<glm::vec3> vert_pos;
    std::vector<glm::vec3> vert_nor;
    std::vector<glm::vec3> vert_col;
    std::vector<GLuint> vert_idx;
    int on_count = 0;
    for (int i = 0; i < count; ++i) {
        glm::vec3 loc = Convert1DCellIndexToLocation(i);
        std::cout << loc.x << ", " << loc.y << ", " << loc.z << std::endl;
//        vert_pos.push_back(loc);
//        vert_nor.push_back(glm::vec3(1.f));

        float val = view_src_trg_lerp == 0 ? grid_cells_info_[i].source_value :
                    view_src_trg_lerp == 1 ? grid_cells_info_[i].target_value :
                                             grid_cells_interp_[i];

        float r = view_src_trg_lerp == 2 ? .5 : 0;

        if (val > 0) {
            vert_col.push_back(glm::vec3(r, val / 3.0f, val / 3.0f));
            vert_pos.push_back(loc);
            vert_nor.push_back(glm::vec3(1.f));
            vert_idx.push_back(on_count);
            ++on_count;
        } else {
//            vert_col.push_back(glm::vec3(-val/10.0f, 0, 0));
        }
//        vert_idx.push_back(i);
    }

    count = vert_col.size();

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(vert_idx.data(), count * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(vert_pos.data(), count * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(vert_col.data(), count * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(vert_nor.data(), count * sizeof(glm::vec3));
}
