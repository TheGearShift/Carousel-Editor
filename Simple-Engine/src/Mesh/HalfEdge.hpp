#pragma once

#include <glm/glm.hpp>

class HalfEdge;

class Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    HalfEdge* incidentEdge;

    Vertex(glm::vec3 pos = glm::vec3(0.0f));
    ~Vertex();
};

class Face {
public:
    HalfEdge* halfEdge;
    glm::vec3 normal;

    Face(HalfEdge* edge = NULL);
    ~Face();
};

class HalfEdge {
public:
    uint32_t vertexIndex;
    Face* face;
    HalfEdge* next;
    HalfEdge* prev;
    HalfEdge* twin;

    HalfEdge();
    ~HalfEdge();
};