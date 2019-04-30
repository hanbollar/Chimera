#ifndef GRID_H
#define GRID_H

#include "globals.h"
#include "bounds.h"

struct Vertex {
    Vertex() {}
    Vertex(const glm::vec3& c, const float& sval, const float& tval)
        : color(c), source_value(sval), target_value(tval) {}
    glm::vec3 color = glm::vec3(1.f);
    float source_value;
    float target_value;
};

class Grid : public Drawable
{
private:
    const glm::vec3 OFFSET = glm::vec3(0.5f);
    Bounds3f bbox_;
    glm::vec3 dim_of_cell_ = glm::vec3(1.f);
    glm::ivec3 dim_num_cells_ = glm::ivec3(20, 20, 20);

    std::vector<Vertex> grid_cells_info_;
    std::vector<float> grid_cells_interp_;

    void ComputeSignedDistanceFunctions(const Mesh* m, const bool& source);

    glm::vec3 origin_ = glm::vec3(0.f);
public:
    Grid();
    Grid(const std::vector<Triangle*>& source, const std::vector<Triangle*>& target);
    Grid(const Bounds3f& bbox);
    ~Grid();

    void SetOrigin(const glm::vec3& origin) {origin_ = origin;}

    glm::ivec3 Convert1DCellIndexTo3DCellIndex(const int& index);
    int Convert3DCellIndexTo1DCellIndex(const int& i, const int& j, const int& k);

    glm::vec3 Convert3DCellIndexToLocation(const int& i, const int& j, const int& k);
    glm::vec3 Convert3DCellIndexToLocation(const glm::ivec3 indices);
    glm::vec3 Convert1DCellIndexToLocation(const int& i);
    glm::ivec3 ConvertLocationToCellIndex(const glm::vec3& loc);

    void ComputeSourceSignedDistanceFunctions(const Mesh* m_source);
    void ComputeTargetSignedDistanceFunctions(const Mesh* m_source);

    void Update();

    virtual GLenum drawMode() const;
    virtual void create();
    void create(bool b);
};

#endif // GRID_H
