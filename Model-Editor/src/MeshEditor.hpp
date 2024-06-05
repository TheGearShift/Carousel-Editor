#pragma once

#include <Mesh/CustomMesh.hpp>
#include <Mesh/HalfEdge.hpp>
#include <Renderer/OpenGlShader.hpp>
#include <glm/glm.hpp>
#include "Camera.hpp"

class MeshEditor {
public:
    MeshEditor(Camera* camera);
    ~MeshEditor();
    void update();
    void render();
    void loadMesh(const std::string& filePath);
    void saveMesh(const std::string& filePath);
    void moveSelectedVertex(glm::vec3 offset);
    void chooseVertex(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HalfEdge*& choosenVertex);
    bool hasSelectedVertex();

private:
    Camera* camera;

    OpenGlShader* gizmoShader;

    CustomMesh* mesh;
    HalfEdge* hoveredVertex;
    HalfEdge* selectedVertex;

    GLuint VAO, VBO;
};