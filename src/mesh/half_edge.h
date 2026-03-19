#include <map>
#include <Eigen/Dense>
#include "mesh.h"

struct HalfEdge {   // index based
    int targetVertex = -1;
    int next = -1;
    int twin = -1;
    int face = -1;
};

struct HE_Vertex {
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    int halfEdge = -1;
};

struct HE_Face {
    int halfEdge = -1;
};

class HEMesh {
public:
    std::vector<HE_Vertex, Eigen::aligned_allocator<HE_Vertex>> vertices;
    std::vector<HalfEdge> halfEdges;
    std::vector<HE_Face> faces;

    void buildFromMesh(const Mesh& mesh);
    Mesh toMesh() const;

    void traverseNeighbors(int v_idx);

    Eigen::Vector3f computeFaceNormal(int f_idx) const;
    void updateVertexNormals();

    // Laplacian Smoothing
    void smooth(float lambda, int iterations);
};