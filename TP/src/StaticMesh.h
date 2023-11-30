#ifndef STATICMESH_H
#define STATICMESH_H

#include <graphics.h>
#include <TypedBuffer.h>
#include <Vertex.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>

namespace OM3D {

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<u32> indices;
    };

    struct BoundingSphere {
        glm::vec3 center;
        float radius;
    };

    class StaticMesh : NonCopyable {

    public:
        StaticMesh() = default;
        StaticMesh(StaticMesh &&) = default;
        StaticMesh &operator=(StaticMesh &&) = default;

        explicit StaticMesh(const MeshData &data);

        void draw() const;

        BoundingSphere bounding_sphere = {{0.0f, 0.0f, 0.0f}, 0.0f};

    private:
        TypedBuffer<Vertex> _vertex_buffer;
        TypedBuffer<u32> _index_buffer;
    };

}

#endif // STATICMESH_H
