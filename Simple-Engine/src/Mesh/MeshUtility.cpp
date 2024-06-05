#include "MeshUtility.hpp"
#include <fstream>
#include <sstream>
#include <ostream>
#include <map>

bool MeshUtility::ArrayToHalfEdgeStructure(CustomMesh* mesh, const std::vector<glm::vec3>& vertices, const std::vector<std::vector<uint32_t>>& faces) {
    mesh->vertices.reserve(vertices.size());
    mesh->faces.reserve(faces.size());

    for (const auto& vertex : vertices) {
        mesh->vertices.emplace_back(vertex);
    }

    std::map<std::pair<uint32_t, uint32_t>, HalfEdge*> halfEdgeMap;
    std::map<std::pair<uint32_t, uint32_t>, HalfEdge*>::iterator halfEdgeIt;
    std::pair<uint32_t, uint32_t> pair;

    for (const auto& vertexIndices : faces) {
        HalfEdge* firstEdge = NULL;
        HalfEdge* previousEdge = NULL;
        HalfEdge* currentEdge = NULL;
        Face* face = new Face();

        for (uint32_t i = 0; i < vertexIndices.size(); i++) {
            uint32_t currIndex = vertexIndices[i];
            uint32_t nextIndex = vertexIndices[(i + 1) % vertexIndices.size()];

            currentEdge = new HalfEdge();
            currentEdge->vertexIndex = currIndex;

            if (!firstEdge) {
                firstEdge = currentEdge;
                firstEdge->face = face;
                face->halfEdge = firstEdge;
                mesh->faces.push_back(face);
            } else {
                currentEdge->face = face;
                currentEdge->prev = previousEdge;
                previousEdge->next = currentEdge;
            }

            mesh->vertices[currIndex].incidentEdge = currentEdge;
            mesh->halfEdges.push_back(currentEdge);

            pair = { nextIndex, currIndex };
            halfEdgeIt = halfEdgeMap.find(pair);
            if (halfEdgeIt != halfEdgeMap.end()) {
                currentEdge->twin = halfEdgeIt->second;
                halfEdgeIt->second->twin = currentEdge;
            }

            pair = { currIndex, nextIndex };
            halfEdgeIt = halfEdgeMap.find(pair);
            if (halfEdgeIt != halfEdgeMap.end()) {
                return false;
            }
            halfEdgeMap[pair] = currentEdge;
            previousEdge = currentEdge;
        }

        if (firstEdge) {
            firstEdge->prev = currentEdge;
            currentEdge->next = firstEdge;
        }

    }

    size_t size = mesh->halfEdges.size();
    for (size_t i = 0; i < size; i++) {
        HalfEdge* halfEdge = mesh->halfEdges[i];
        if (halfEdge->twin) continue;
        uint32_t v0 = halfEdge->vertexIndex;
        uint32_t v1 = halfEdge->next->vertexIndex;

        HalfEdge* edge = new HalfEdge();
        edge->vertexIndex = v1;
        edge->twin = halfEdge;
        halfEdge->twin = edge;

        mesh->halfEdges.push_back(edge);
    }

    for (const auto& halfEdge : mesh->halfEdges) {
        if (halfEdge->face == NULL) {
            HalfEdge* next = halfEdge->twin;
            do {
                next = next->prev->twin;
            }
            while (next->face != NULL);
            halfEdge->next = next;
            next->prev = halfEdge;
        }
    }
    mesh->normalize();
    return true;
}

void MeshUtility::HalfEdgeStructureToArray(const CustomMesh* mesh, std::vector<glm::vec3>& vertices, std::vector<std::vector<uint32_t>>& faces) {
    for (const auto& vertex : mesh->vertices) {
        vertices.push_back(vertex.position * mesh->getScale());
    }

    std::vector<uint32_t> rawFaces;
    for (const auto& face : mesh->faces) {
        HalfEdge* e3 = face->halfEdge->prev;
        HalfEdge* e1 = face->halfEdge;
        HalfEdge* e2 = face->halfEdge->next;

        rawFaces.push_back(e1->vertexIndex);
        rawFaces.push_back(e2->vertexIndex);
        rawFaces.push_back(e3->vertexIndex);

        faces.push_back(move(rawFaces));
    }
}

bool MeshUtility::ReadObjFile(const std::string& filePath, std::vector<glm::vec3>& vertices, std::vector<std::vector<uint32_t>>& faces) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    std::vector<uint32_t> indices;
    std::string line;
    while (getline(file, line)) {
        std::istringstream lineStream(line);
        std::string token;
        lineStream >> token;

        if (token == "v") {
            glm::vec3 vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (token == "f") {
            std::string faceElement;
            while (lineStream >> faceElement) {
                std::istringstream elementStream(faceElement);
                std::string index;
                if (std::getline(elementStream, index, '/') && index != "") {
                    indices.push_back(std::stoi(index) - 1);
                }
            }
            faces.push_back(move(indices));
        }
    }
    file.close();
    return true;
}

void MeshUtility::WriteObjFile(const std::string& filePath, const std::vector<glm::vec3>& vertices, const std::vector<std::vector<uint32_t>>& faces) {
    std::ostringstream lineStream;

    lineStream << "# Vertices : " << vertices.size() << "\n";
    lineStream << "# Faces : " << faces.size() << "\n";

    for (const auto& vertex : vertices) {
        lineStream << "\nv";
        lineStream << " " << vertex.x;
        lineStream << " " << vertex.y;
        lineStream << " " << vertex.z;
    }

    for (const auto& face : faces) {
        lineStream << "\nf";
        for (const auto& index : face) {
            lineStream << " " << index + 1;
        }
    }

    std::ofstream file(filePath);
    file << lineStream.str();
    file.close();
}