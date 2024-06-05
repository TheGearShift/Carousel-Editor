#include "CustomMesh.hpp"
#include <Core/FileDialog.hpp>
#include <Mesh/MeshUtility.hpp>

CustomMesh* CustomMesh::Create(const std::string& filePath) {
    std::vector<glm::vec3> vertices;
    std::vector<std::vector<uint32_t>> rawFaces;
    if (!MeshUtility::ReadObjFile(filePath, vertices, rawFaces)) {
        return NULL;
    }

    std::vector<std::vector<uint32_t>> faces;
    std::vector<uint32_t> face;
    for (const auto& vertexIndices : rawFaces) {
        face.push_back(vertexIndices[0]);
        face.push_back(vertexIndices[1]);
        face.push_back(vertexIndices[2]);
        faces.push_back(move(face));
        for (uint32_t i = 3; i < vertexIndices.size(); i++) {
            face.push_back(vertexIndices[i - 3]);
            face.push_back(vertexIndices[i - 1]);
            face.push_back(vertexIndices[i]);
            faces.push_back(move(face));
        }
    }

    CustomMesh* mesh = new CustomMesh();
    if (!MeshUtility::ArrayToHalfEdgeStructure(mesh, vertices, faces)) {
        FileDialog::ShowWarningMessage("Invalid File", "Selected model has duplicated edges!");
        delete(mesh);
        return NULL;
    }

    return mesh;
}

CustomMesh::CustomMesh() {
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);
}

CustomMesh::~CustomMesh() {
    for (const auto& face : faces) {
        delete(face);
    }
    for (const auto& halfEdge : halfEdges) {
        delete(halfEdge);
    }
    faces.clear();
    halfEdges.clear();

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &nbo);
}

void CustomMesh::updateMeshGeometry() {
    std::vector<glm::vec3> meshVertices;
    std::vector<glm::vec3> meshNormals;

    for (const auto& face : faces) {
        HalfEdge* startEdge = face->halfEdge;
        HalfEdge* currentEdge = startEdge;
        do {
            meshVertices.push_back(vertices[currentEdge->vertexIndex].position);
            currentEdge = currentEdge->next;
        }
        while (currentEdge != startEdge);

        glm::vec3& v0 = vertices[startEdge->prev->vertexIndex].position;
        glm::vec3& v1 = vertices[startEdge->vertexIndex].position;
        glm::vec3& v2 = vertices[startEdge->next->vertexIndex].position;
        glm::vec3 normal = glm::cross(v0 - v1, v1 - v2);
        normal = glm::normalize(normal);
        face->normal = normal;

        meshNormals.push_back(normal);
        meshNormals.push_back(normal);
        meshNormals.push_back(normal);
    }

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * meshVertices.size(), meshVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * meshNormals.size(), meshNormals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CustomMesh::render() {
    if (vertexArrayObject != 0) {
        if (shouldRebuildMesh) {
            updateMeshGeometry();
            shouldRebuildMesh = false;
        }

        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, GLsizei(faces.size() * 3));
        glBindVertexArray(0);
    }
}

void CustomMesh::normalize() {
    if (vertices.size() > 0) {
        glm::vec3 v = vertices[0].position;
        glm::vec3 vmin = v, vmax = v;

        for (const auto& v : vertices) {
            if (v.position.x < vmin.x) vmin.x = v.position.x;
            if (v.position.x > vmax.x) vmax.x = v.position.x;

            if (v.position.y < vmin.y) vmin.y = v.position.y;
            if (v.position.y > vmax.y) vmax.y = v.position.y;

            if (v.position.z < vmin.z) vmin.z = v.position.z;
            if (v.position.z > vmax.z) vmax.z = v.position.z;
        }

        glm::vec3 dist = vmax - vmin;
        glm::vec3 diff = (vmax + vmin) / 2.0f;
        scale = std::max({ dist.x, dist.y, dist.z });

        for (auto& v : vertices) {
            v.position -= diff;
            v.position /= scale;
        }
    }
}

void CustomMesh::rebuildMesh() { shouldRebuildMesh = true; }

float CustomMesh::getScale() const { return scale; }