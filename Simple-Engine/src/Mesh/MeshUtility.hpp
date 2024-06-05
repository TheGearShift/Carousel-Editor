#pragma once

#include <Mesh/CustomMesh.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class MeshUtility {
public:
    static bool ArrayToHalfEdgeStructure(
        CustomMesh* mesh,
        const std::vector<glm::vec3>& vertices,
        const std::vector<std::vector<uint32_t>>& faces
    );

    static void HalfEdgeStructureToArray(
        const CustomMesh* mesh,
        std::vector<glm::vec3>& vertices,
        std::vector<std::vector<uint32_t>>& faces
    );

    static bool ReadObjFile(
        const std::string& filePath,
        std::vector<glm::vec3>& vertices,
        std::vector<std::vector<uint32_t>>& faces
    );

    static void WriteObjFile(
        const std::string& filePath,
        const std::vector<glm::vec3>& vertices,
        const std::vector<std::vector<uint32_t>>& faces
    );
};