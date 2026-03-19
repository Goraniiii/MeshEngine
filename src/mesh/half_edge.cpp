#include "half_edge.h"

#include <iostream>

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

    // key: {v_start, v_end}, value: edge_index
    std::map<std::pair<int, int>, int> edgeMap;

    // 2. HalfEdge and Face
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        int face_idx = faces.size();
        faces.push_back({ (int)halfEdges.size() });

        int v_indices[3] = { (int)mesh.indices[i], (int)mesh.indices[i + 1], (int)mesh.indices[i + 2] };
        int start_he_idx = halfEdges.size();

        for (int j = 0; j < 3; j++) {
            int v_curr = v_indices[j];
            int v_next = v_indices[(j + 1) % 3];

            HalfEdge he;
            he.targetVertex = v_next;
            he.face = face_idx;
            he.next = start_he_idx + ((j + 1) % 3);
            he.twin = -1;  // 초기화 명시

            if (vertices[v_curr].halfEdge == -1) {
                vertices[v_curr].halfEdge = start_he_idx + j;
            }

            int current_he_idx = (int)halfEdges.size();
            halfEdges.push_back(he);

            auto twinKey = std::make_pair(v_next, v_curr);
            if (edgeMap.find(twinKey) != edgeMap.end()) {
                int twin_idx = edgeMap[twinKey];

                he.twin = twin_idx;
                halfEdges[twin_idx].twin = current_he_idx;
            }

            edgeMap[std::make_pair(v_curr, v_next)] = current_he_idx;
        }
    }

    int twinCount = 0;
    for (const auto& he : halfEdges) {
        if (he.twin != -1) twinCount++;
    }
    std::cout << "Total HalfEdges: " << halfEdges.size() << ", Connected Twins: " << twinCount << std::endl;
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

    // 2. Index (faces traverse)
    result.indices.reserve(this->faces.size() * 3);
    for (const auto& face : this->faces) {
        if (face.halfEdge == -1) { continue; }

        int e0 = face.halfEdge;
        int e1 = halfEdges[e0].next;
        int e2 = halfEdges[e1].next;

        int v0 = halfEdges[e2].targetVertex;
        int v1 = halfEdges[e0].targetVertex;
        int v2 = halfEdges[e1].targetVertex;

        result.indices.push_back(v0);
        result.indices.push_back(v1);
        result.indices.push_back(v2);
    }

    return result;
}

void HEMesh::traverseNeighbors(int v_idx) {
    int startEdge = vertices[v_idx].halfEdge;
    if (startEdge == -1) { return; }    // 연결된 edge가 없는 case

    int curr = startEdge;
    do {
        // 1. face
        int f_idx = halfEdges[curr].face;
        if (f_idx != -1) {
            std::cout << "visited face: " << f_idx << std::endl;
        }

        // 2. neighbor vertex
        int neighbor_v = halfEdges[curr].targetVertex;
        std::cout << "neighbor vertex: " << neighbor_v << std::endl;

        // 3. next edge
        int twin_idx = halfEdges[curr].twin;
        if (twin_idx == -1) { break; }

        curr = halfEdges[twin_idx].next;

    } while (curr != startEdge);
}

Eigen::Vector3f HEMesh::computeFaceNormal(int f_idx) const {
    int e0 = faces[f_idx].halfEdge;
    int e1 = halfEdges[e0].next;
    int e2 = halfEdges[e1].next;

    // position (e2->v0, e0->v1, e1->v2)
    const Eigen::Vector3f& v0 = vertices[halfEdges[e2].targetVertex].position;
    const Eigen::Vector3f& v1 = vertices[halfEdges[e0].targetVertex].position;
    const Eigen::Vector3f& v2 = vertices[halfEdges[e1].targetVertex].position;

    Eigen::Vector3f side1 = v1 - v0;
    Eigen::Vector3f side2 = v2 - v0;

    Eigen::Vector3f normal = side1.cross(side2).normalized(); // 외적 후 정규화

    return normal;
}

void HEMesh::updateVertexNormals() {
    for (int i = 0; i < vertices.size(); ++i) {
        Eigen::Vector3f avgNormal(0, 0, 0);

        int startEdge = vertices[i].halfEdge;
        if (startEdge == -1) { continue; }

        int curr = startEdge;
        do {
            if (halfEdges[curr].face != -1) {
                avgNormal += computeFaceNormal(halfEdges[curr].face);
            }

            int prev_he = halfEdges[halfEdges[curr].next].next;
            curr = halfEdges[prev_he].twin;

            if (curr == -1) { break; }

        } while (curr != startEdge);

        vertices[i].normal = avgNormal.normalized();
    }
}

void HEMesh::applyLaplacian(float lambda) {
    // Double Buffering
    std::vector<Eigen::Vector3f> newPositions(vertices.size());

    for (int i = 0; i < vertices.size(); ++i) {
        int startEdge = vertices[i].halfEdge;
        if (startEdge == -1) {
            newPositions[i] = vertices[i].position;
            continue;
        }

        Eigen::Vector3f sumNeighborPos(0, 0, 0);
        int neighborCount = 0;

        // Half-Edge traverse
        int curr = startEdge;
        do {
            int neighborIdx = halfEdges[curr].targetVertex;
            sumNeighborPos += vertices[neighborIdx].position;
            neighborCount++;

            int prevEdge = halfEdges[halfEdges[curr].next].next;
            curr = halfEdges[prevEdge].twin;

            if (curr == -1) { break; }

        } while (curr != startEdge);

        if (neighborCount > 0) {
            Eigen::Vector3f averagePos = sumNeighborPos / (float)neighborCount;
            newPositions[i] = vertices[i].position + lambda * (averagePos - vertices[i].position);
        }
        else {
            newPositions[i] = vertices[i].position;
        }
    }
           
    // update positions
    for (int i = 0; i < vertices.size(); ++i) {
        vertices[i].position = newPositions[i];
    }
}

void HEMesh::smoothLaplacian(float lambda, int iterations) {
    for (int iter = 0; iter < iterations; ++iter) {
        applyLaplacian(lambda);
    }

    updateVertexNormals();
}

void HEMesh::smoothTaubin(float lambda, float mu, int iterations) {
    for (int iter = 0; iter < iterations; ++iter) {
        applyLaplacian(lambda);

        applyLaplacian(mu);
    }

    updateVertexNormals();
}