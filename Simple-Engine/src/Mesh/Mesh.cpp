#include "Mesh.hpp"

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::render() {
    if (vertexArrayObject != 0 && indexCount != 0) {
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}