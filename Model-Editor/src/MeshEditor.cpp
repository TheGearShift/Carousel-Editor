#include "MeshEditor.hpp"
#include <Core/Application.hpp>
#include <Input/Input.hpp>
#include <Mesh/MeshUtility.hpp>
#include <glm/gtc/matrix_transform.hpp>

MeshEditor::MeshEditor(Camera* camera)
    : camera(camera), mesh(NULL), hoveredVertex(NULL), selectedVertex(NULL) {
    const float length = 0.01f;
    const float vertices[] =
    {
        length, -length, 0.0f,
        length, length, 0.0f,
        -length, length, 0.0f,

        length, -length, 0.0f,
        -length, length, 0.0f,
        -length, -length, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    gizmoShader = new OpenGlShader("../../../shaders/basicGizmoShader.glsl");
}

MeshEditor::~MeshEditor() {
    if (mesh) {
        delete(mesh);
    }
    delete(gizmoShader);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void MeshEditor::update() {
    if (!mesh) return;

    Window& window = Application::Get().getWindow();
    MousePosition mousePosition = Input::GetMousePosition();
    glm::vec4 rayNds = glm::vec4(
        (2.0 * mousePosition.x) / window.getWidth() - 1.0,
        1.0 - (2.0 * mousePosition.y) / window.getHeight(),
        -1.0f,
        1.0f
    );
    glm::vec4 rayCameraCoords = glm::inverse(camera->getPerspective()) * rayNds;
    rayCameraCoords = glm::vec4(rayCameraCoords.x, rayCameraCoords.y, -1.0f, 0.0f);
    glm::vec3 rayDirection = glm::inverse(camera->getView()) * rayCameraCoords;
    rayDirection = glm::normalize(rayDirection);
    if (Input::IsLeftMouseClicked()) {
        chooseVertex(camera->getPosition(), rayDirection, selectedVertex);
    }
    chooseVertex(camera->getPosition(), rayDirection, hoveredVertex);
}

void MeshEditor::render() {
    if (!mesh) return;

    mesh->render();

    GLint  wireFrameMode;
    glGetIntegerv(GL_POLYGON_MODE, &wireFrameMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    if (hoveredVertex) {
        glm::vec3& point = mesh->vertices[hoveredVertex->vertexIndex].position;
        glm::mat4 viewDirection = glm::lookAt(point, point + camera->getDirection(), cameraUp);
        float dist = glm::length(point - camera->getPosition());
        glm::mat4 scalar = glm::scale(glm::mat4(1.0f), glm::vec3(dist));
        gizmoShader->useShader();
        gizmoShader->setMat4("projection", camera->getPerspective());
        gizmoShader->setMat4("view", camera->getView());
        gizmoShader->setMat4("model", glm::inverse(viewDirection) * scalar);
        gizmoShader->setVec3("u_color", glm::vec3(0.607f, 0.678f, 0.717f));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    if (selectedVertex) {
        glm::vec3& point = mesh->vertices[selectedVertex->vertexIndex].position;
        glm::mat4 viewDirection = glm::lookAt(point, point + camera->getDirection(), cameraUp);
        float dist = glm::length(point - camera->getPosition());
        glm::mat4 scalar = glm::scale(glm::mat4(1.0f), glm::vec3(dist));
        gizmoShader->useShader();
        gizmoShader->setMat4("projection", camera->getPerspective());
        gizmoShader->setMat4("view", camera->getView());
        gizmoShader->setMat4("model", glm::inverse(viewDirection) * scalar);
        gizmoShader->setVec3("u_color", glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode);
}

void MeshEditor::loadMesh(const std::string& filePath) {
    CustomMesh* mesh = CustomMesh::Create(filePath);
    if (!mesh) return;

    if (this->mesh) {
        delete(this->mesh);
    }

    this->mesh = mesh;
    hoveredVertex = NULL;
    selectedVertex = NULL;
}

void MeshEditor::saveMesh(const std::string& filePath) {
    if (!mesh) return;

    std::vector<glm::vec3> vertices;
    std::vector<std::vector<uint32_t>> faces;
    MeshUtility::HalfEdgeStructureToArray(mesh, vertices, faces);
    MeshUtility::WriteObjFile(filePath, vertices, faces);
}

void MeshEditor::moveSelectedVertex(glm::vec3 offset) {
    if (!selectedVertex) return;

    offset /= mesh->getScale();
    mesh->vertices[selectedVertex->vertexIndex].position += offset;
    mesh->rebuildMesh();
}

void MeshEditor::chooseVertex(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HalfEdge*& choosenVertex) {
    const float r = 0.02f * camera->offsetMultiplier;
    choosenVertex = NULL;
    for (const auto& vertex : mesh->vertices) {
        glm::vec3 L = vertex.position - rayOrigin;
        float tca = glm::dot(L, rayDirection);

        if (tca < 0) continue;

        float d = std::sqrt(glm::dot(L, L) - (tca * tca));

        if (d < 0 || d > r) continue;

        HalfEdge* startEdge = vertex.incidentEdge;
        HalfEdge* currentEdge = startEdge;
        do {
            Face* face = currentEdge->face;

            if (face && glm::dot(L, face->normal) < 0) {
                choosenVertex = currentEdge;
                return;
            }

            currentEdge = currentEdge->prev->twin;
        }
        while (currentEdge != startEdge);
    }
}

bool MeshEditor::hasSelectedVertex() { return selectedVertex != NULL; }