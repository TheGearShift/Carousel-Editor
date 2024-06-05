#include "HalfEdge.hpp"

Vertex::Vertex(glm::vec3 pos) {
    position = pos;
    incidentEdge = NULL;
}

Vertex::~Vertex() {}

Face::Face(HalfEdge* edge) {
    halfEdge = edge;
    normal = glm::vec3(0.0f);
}

Face::~Face() {}

HalfEdge::HalfEdge() {
    vertexIndex = 0;
    face = NULL;
    next = NULL;
    prev = NULL;
    twin = NULL;
}

HalfEdge::~HalfEdge() {}