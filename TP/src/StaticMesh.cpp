#include "StaticMesh.h"

#include <glad/glad.h>

namespace OM3D {

    extern bool audit_bindings_before_draw;

    StaticMesh::StaticMesh(const MeshData &data) : _vertex_buffer(data.vertices), _index_buffer(data.indices) {
        glm::vec3 sphereCenter = glm::vec3(0.0f);
        float radius = 0.0f;
        for (const auto &vertex : data.vertices) {
            sphereCenter += vertex.position;
        }
        sphereCenter /= float(data.vertices.size());
        for (const auto &vertex : data.vertices) {
            radius = std::max(radius, static_cast<float>(glm::length(vertex.position - sphereCenter)));
        }

//        std::cout << "Bounding sphere center: " << sphereCenter.x << ", " << sphereCenter.y << ", " << sphereCenter.z << std::endl;
//        std::cout << "Bounding sphere radius: " << radius << std::endl;
        bounding_sphere = {sphereCenter, radius};
    }

    void StaticMesh::draw(size_t n) const {
        _vertex_buffer.bind(BufferUsage::Attribute);
        _index_buffer.bind(BufferUsage::Index);

        // Vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);
        // Vertex normal
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));
        // Vertex uv
        glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void *>(6 * sizeof(float)));
        // Tangent / bitangent sign
        glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void *>(8 * sizeof(float)));
        // Vertex color
        glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void *>(12 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);

        if (audit_bindings_before_draw) {
            audit_bindings();
        }

        glDrawElementsInstanced(GL_TRIANGLES, int(_index_buffer.element_count()), GL_UNSIGNED_INT, nullptr, int(n));
    }
}
