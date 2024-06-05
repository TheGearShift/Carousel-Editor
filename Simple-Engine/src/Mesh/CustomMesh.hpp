#pragma once

#include <Mesh/Mesh.hpp>
#include <Mesh/HalfEdge.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class CustomMesh : public Mesh {
public:
    static CustomMesh* Create(const std::string& filePath);

    CustomMesh();
    ~CustomMesh();
    void updateMeshGeometry();
    void render() override;
    void normalize();
    void rebuildMesh();
    float getScale() const;

    std::vector<Vertex> vertices;
    std::vector<Face*> faces;
    std::vector<HalfEdge*> halfEdges;

private:
    GLuint vbo, nbo;
    float scale;
    bool shouldRebuildMesh;
};