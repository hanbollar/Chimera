#include "scene/grid.h"
#include "tableinfo.h"

Grid::Grid() :
    bbox_(Bounds3f()),
    dim_of_cell_(glm::vec3(-1)),
    grid_cells_info_(std::vector<Vertex>()),
    grid_cells_interp_(std::vector<float>())
{}

Grid::Grid(const Bounds3f &bbox) :
    bbox_(bbox), dim_of_cell_(glm::vec3(0.f)),
    grid_cells_info_(std::vector<Vertex>(dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z, Vertex())),
    grid_cells_interp_(std::vector<float>(dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z, 0))
{
    dim_of_cell_ = (2.f*OFFSET + bbox.max - bbox.min) / glm::vec3(dim_num_cells_);
    origin_ = bbox.min - OFFSET;
}

Grid::Grid(const std::vector<Triangle*>& source, const std::vector<Triangle*>& target) : 
    bbox_(Bounds3f::Union(*Bounds3f::BuildBoundingBox(source), *Bounds3f::BuildBoundingBox(target))), dim_of_cell_(glm::vec3(0.f)),
    grid_cells_info_(std::vector<Vertex>(dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z, Vertex())),
    grid_cells_interp_(std::vector<float>(dim_num_cells_.x * dim_num_cells_.y * dim_num_cells_.z, 0))
{
    dim_of_cell_ = (2.f*OFFSET + bbox_.max - bbox_.min) / glm::vec3(dim_num_cells_);
    origin_ = bbox_.min - OFFSET;
}

Grid::Grid(const std::vector<Triangle*>& source, const std::vector<Triangle*>& target, int dimension) :
    bbox_(Bounds3f::Union(*Bounds3f::BuildBoundingBox(source), *Bounds3f::BuildBoundingBox(target))), dim_of_cell_(glm::vec3(0.f)),
    grid_cells_info_(std::vector<Vertex>(dimension * dimension * dimension, Vertex())),
    grid_cells_interp_(std::vector<float>(dimension * dimension * dimension, 0))
{
    dim_of_cell_ = (2.f*OFFSET + bbox_.max - bbox_.min) / glm::vec3(dimension);
    origin_ = bbox_.min - OFFSET;
    dim_num_cells_ = glm::ivec3(dimension, dimension, dimension);
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
                }
                else if (i == dim_num_cells_.x - 1) {
                    POS_offset.x = 0;
                }

                // edge cases for y
                if (j == 0) {
                    NEG_offset.y = 0;
                }
                else if (j == dim_num_cells_.y - 1) {
                    POS_offset.y = 0;
                }

                // edge cases for z
                if (k == 0) {
                    NEG_offset.z = 0;
                }
                else if (k == dim_num_cells_.z - 1) {
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
                float front_top_lerpx = (front_val2 + front_val3) / 2.f;
                // lerp along y - 2 vals
                float front_mid_lerpy = (front_bot_lerpx + front_top_lerpx) / 2.f;

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
        }
        else {
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

Mesh* Grid::BuildMesh() {
    const float ISOLEVEL_THRESHOLD = 0.f;

    std::vector<Triangle*> faces_geometry;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    for (unsigned int i = 0; i < grid_cells_interp_.size(); ++i) {
        glm::ivec3 cell_index = Convert1DCellIndexTo3DCellIndex(i);
        if (cell_index.x == this->dim_num_cells_.x - 1 ||
            cell_index.y == this->dim_num_cells_.y - 1 ||
            cell_index.z == this->dim_num_cells_.z - 1) {

            // voxelizing from [0->bound - 1). don't want to add additional voxels on far sides.
            continue;
        }

        // testing
        if (grid_cells_interp_[i] > 0) {
            auto temp = grid_cells_interp_[i];
            std::cout << "stop " << std::endl;
        }

        // on voxel i
        Polygonize(ISOLEVEL_THRESHOLD, cell_index, positions, normals);

    }

    // faces and faces normals
    for (unsigned int i = 0; i < positions.size(); i += 3) {
        faces_geometry.push_back(new Triangle(positions[i],
            positions[i + 1],
            positions[i + 2]));
        //        faces_geometry[faces_geometry.size() - 1]->normals[0] = normals[i];
        //        faces_geometry[faces_geometry.size() - 1]->normals[1] = normals[i+1];
        //        faces_geometry[faces_geometry.size() - 1]->normals[2] = normals[i+2];
    }

    std::cout << "FINISHED CREATING GRID MESH" << std::endl;
    return new Mesh(faces_geometry);
}

glm::vec3 Grid::VertexInterpolation(const float& isolevel,
    const glm::vec3& posA, const glm::vec3& posB,
    const float& ip1, const float& ip2) {
    /********************************************************************************************************/
    // compute the linearly interpolated vertex position on the edge according to the isovalue at each
    // end corner of the edge.
    /********************************************************************************************************/
    float epsilon = 0.00001;

    // just p1
    if (glm::abs(isolevel - ip1) < epsilon) {
        return posA;
    }
    // just p2
    if (glm::abs(isolevel - ip2) < epsilon) {
        return posB;
    }
    // based on diff between isovals of the points, if too close then just return one of them
    // bc almost as if the same
    if (glm::abs(ip1 - ip2) < epsilon) {
        return posA;
    }

    glm::vec3 p(0.0, 0.0, 0.0);
    float weight = (isolevel - ip1) / (ip2 - ip1);
    p.x = posA.x + weight * (posB.x - posA.x);
    p.y = posA.y + weight * (posB.y - posA.y);
    p.z = posA.z + weight * (posB.z - posA.z);

    return p;
}

glm::vec3 ComputeNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    glm::vec3 ab = b - a;
    glm::vec3 cb = b - c;
    return glm::normalize(glm::cross(ab, cb));
}

glm::vec3 LERP(const glm::vec3& a, const glm::vec3& b, const float& t) {
    return a * (1 - t) + b * t;
}

#define DEBUG1
#define DEBUG2

//  http://paulbourke.net/geometry/polygonise/
void Grid::Polygonize(const float& isoLevel, const glm::ivec3& index3D, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals) {
    /********************************************************************************************************/
    // compute the vertex positions based on intersected edges and isovals. from these created polygons
    // compute the associated normals as well.
    /********************************************************************************************************/

//    if (Convert3DCellIndexToLocation(index3D.x, index3D.y, index3D.z) == glm::vec3(0)) {
//        std::cout << "stop here " <<std::endl;
//    }

    // figure out cell configuration based isoVals versus isoLevel
    // drawing the voxel from [0->cellwidth) in all 3 dimensions
    const float isoSamples[] = { grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(index3D.x,     index3D.y,      index3D.z + 1)], // bot face: 0, 1, 2, 3
                                    grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(index3D.x + 1, index3D.y,      index3D.z + 1)],
                                    grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(index3D.x + 1, index3D.y,      index3D.z)],
                                    grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(index3D.x,     index3D.y,      index3D.z)],
                                    grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(index3D.x,     index3D.y + 1,  index3D.z + 1)], // top face: 4, 5, 6, 7
                                    grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(index3D.x + 1, index3D.y + 1,  index3D.z + 1)],
                                    grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(index3D.x + 1, index3D.y + 1,  index3D.z)],
                                    grid_cells_interp_[Convert3DCellIndexTo1DCellIndex(index3D.x,     index3D.y + 1,  index3D.z)] };
    int pickCube = 0;
    for (unsigned i = 0; i < 8; ++i) {
        if (isoSamples[i] > isoLevel) {
            pickCube += std::pow(2, i);
        }
    }

    // finding vertices where cube intersects
    // if cube index is 0 then no vertices returned [cube entirely in/out of the visual area so not shown]
    if (pickCube == 0) {
        return;
    }

    // GIVEN POSITIONS
    glm::vec3 base_corner = this->Convert3DCellIndexToLocation(index3D);

    static const glm::vec3 allPoints[] = { glm::vec3(base_corner.x,                   base_corner.y,                    base_corner.z + dim_of_cell_.z), // bottom face: 0, 1, 2, 3
                                           glm::vec3(base_corner.x + dim_of_cell_.x,  base_corner.y,                    base_corner.z + dim_of_cell_.z),
                                           glm::vec3(base_corner.x + dim_of_cell_.x,  base_corner.y,                    base_corner.z),
                                           glm::vec3(base_corner.x,                   base_corner.y,                    base_corner.z),
                                           glm::vec3(base_corner.x,                   base_corner.y + dim_of_cell_.y,   base_corner.z + dim_of_cell_.z), // top face: 4, 5, 6, 7
                                           glm::vec3(base_corner.x + dim_of_cell_.x,  base_corner.y + dim_of_cell_.y,   base_corner.z + dim_of_cell_.z),
                                           glm::vec3(base_corner.x + dim_of_cell_.x,  base_corner.y + dim_of_cell_.y,   base_corner.z),
                                           glm::vec3(base_corner.x,                   base_corner.y + dim_of_cell_.y,   base_corner.z) };

    /*  int checking_indices[] = { 0, 1, 1, 2, 2, 3, 3, 0,   // bottom
                                 4, 5, 5, 6, 6, 7, 7, 4,   // top
                                       3, 7,               // front (to viewer)
                                       2, 6,             // back (to viewer)
                                                   4, 0,   // left
                                                   1, 5 }; // right
      const int checking_indices_size = 12;
      std::vector<glm::vec3> added_points;
      // triangulate face?
      for (int i = 0; i < checking_indices_size; i += 2) {
          int i_0 = checking_indices[i];
          int i_1 = checking_indices[i+1];
          if (glm::sign(isoSamples[i_0]) != glm::sign(isoSamples[i_1])) {
              float a = isoSamples[i_0];
              float b = isoSamples[i_1];
              float dist = abs(a - b);
              // t = min / dist
              float t = abs(a / dist);
              //added_points.push_back(allPoints[i_0] * (1 - t) + allPoints[i_1] * t);

              added_points.push_back(VertexInterpolation(0, allPoints[i_0], allPoints[i_1], isoSamples[i_0], isoSamples[i_1]));
          }
      }

      if (added_points.size() > 0) {
          if (added_points.size() == 1) {
              // ignore - will be hit in a different iteration
          }
          for (unsigned int i = 0; i < added_points.size(); ++i) {
              positions.push_back(added_points[i]);
          }
          std::cout << "ADDED GRID MESH VERTS: " << added_points.size() << std::endl;
      }

      // --- OLD IMPL
      return;*/

    int indexLocs[] = { 0, 1, 1, 2,
                        2, 3, 3, 0,
                        4, 5, 5, 6,
                        6, 7, 7, 4,
                        0, 4, 1, 5,
                        2, 6, 3, 7 };

    // i goes from [0, 11] bc going through all poss bits
    __int32 twelveBits = EDGE_TABLE[pickCube];

    std::vector<glm::vec3> lerpingVals(12, glm::vec3(0));

    unsigned int j = 0;
    for (int i = 0; i < 12; ++i) {
        if (twelveBits & (int)std::pow(2, i)) {
            unsigned int i_curr = indexLocs[j];
            unsigned int i_next = indexLocs[j + 1];

            lerpingVals[i] = VertexInterpolation(isoLevel,
                allPoints[i_curr], allPoints[i_next],
                isoSamples[i_curr], isoSamples[i_next]);
        }

        j += 2;
    }//end: forLoop over 12bits

    // looping for vertex and normal list positions until hitting -1
    for (unsigned int i = 0; TRI_TABLE[pickCube * 16 + i] != -1; ++i) {
        positions.push_back(base_corner + lerpingVals[TRI_TABLE[pickCube * 16 + i]]);
    }
    /*
        //---- OLD OLD IMPL
        return;

        int indexLocs[] = { 0, 1, 1, 2,
                            2, 3, 3, 0,
                            4, 5, 5, 6,
                            6, 7, 7, 4,
                            0, 4, 1, 5,
                            2, 6, 3, 7 };

        // i goes from [0, 11] bc going through all poss bits
        __int32 twelveBits = EDGE_TABLE[pickCube];

        std::vector<glm::vec3> lerpingVals(12, glm::vec3(0));

        unsigned int j = 0;
        for (int i = 0; i < 12; ++i) {
          if (twelveBits & (int)std::pow(2, i)) {
            unsigned int i_curr = indexLocs[j];
            unsigned int i_next = indexLocs[j+1];

            lerpingVals[i] = VertexInterpolation(isoLevel,
                                                 allPoints[i_curr], allPoints[i_next],
                                                 isoSamples[i_curr], isoSamples[i_next]);
          }

          j += 2;
        }//end: forLoop over 12bits

        // looping for vertex and normal list positions until hitting -1
        for (unsigned int i = 0; TRI_TABLE[pickCube * 16 + i] != -1; ++i) {
          positions.push_back(base_corner + lerpingVals[TRI_TABLE[pickCube * 16 + i]]);
        }*/
        /*glm::vec3 temp_nor = ComputeNormal( positions[positions.size() - 3],
                                            positions[positions.size() - 2],
                                            positions[positions.size() - 1]);
        normals.push_back(temp_nor);
        normals.push_back(temp_nor);
        normals.push_back(temp_nor);*/
}
