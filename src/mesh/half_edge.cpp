#include "half_edge.h"

void HEMesh::buildFromMesh(const Mesh& mesh) {
    vertices.clear();
    halfEdges.clear();
    faces.clear();

    // 1. copy vertices
    for (const auto& v : mesh.vertices) {
        HE_Vertex hev;
        hev.position = v.position;
        hev.normal = v.normal;
        vertices.push_back(hev);
    }

    // Twin Edge map (key: {v_start, v_end}, value: edge_index)
    std::map<std::pair<int, int>, int> edgeMap;

    // 2. Face and Half-Edge
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        int f_idx = faces.size();
        faces.push_back({ (int)halfEdges.size() });

        int v_indices[3] = { (int)mesh.indices[i], (int)mesh.indices[i + 1], (int)mesh.indices[i + 2] };
        int start_he_idx = halfEdges.size();

        for (int j = 0; j < 3; ++j) {
            int v_curr = v_indices[j];
            int v_next = v_indices[(j + 1) % 3];

            HalfEdge he;
            he.targetVertex = v_next;
            he.face = f_idx;
            he.next = start_he_idx + (j + 1) % 3;

            vertices[v_curr].halfEdge = start_he_idx + j;

            auto twinKey = std::make_pair(v_next, v_curr);
            if (edgeMap.find(twinKey) != edgeMap.end()) {
                int twin_idx = edgeMap[twinKey];
                he.twin = twin_idx;
                halfEdges[twin_idx].twin = start_he_idx + j;
            }

            edgeMap[std::make_pair(v_curr, v_next)] = start_he_idx + j;
            halfEdges.push_back(he);
        }
    }
}

Mesh HEMesh::toMesh() const {
    Mesh result;

    // 1. copy vertices
    result.vertices.reserve(this->vertices.size());
    for (const auto& hev : this->vertices) {
        Vertex v;
        v.position = hev.position;
        v.normal = hev.normal;
        result.vertices.push_back(v);
    }

    // 2. index
    result.indices.reserve(this->faces.size() * 3);
    for (const auto& face : this->faces) {
        if (face.halfEdge == -1) continue;

        int e0 = face.halfEdge;
        int e1 = halfEdges[e0].next;
        int e2 = halfEdges[e1].next;

        result.indices.push_back(halfEdges[e2].targetVertex);
        result.indices.push_back(halfEdges[e0].targetVertex);
        result.indices.push_back(halfEdges[e1].targetVertex);
    }

    return result;
}