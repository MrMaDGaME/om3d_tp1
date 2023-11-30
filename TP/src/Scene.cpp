#include "Scene.h"

#include <TypedBuffer.h>

#include <shader_structs.h>

namespace OM3D {

    Scene::Scene() {
    }

    void Scene::add_object(SceneObject obj) {
        _objects.emplace_back(std::move(obj));
    }

    void Scene::add_light(PointLight obj) {
        _point_lights.emplace_back(std::move(obj));
    }

    Span<const SceneObject> Scene::objects() const {
        return _objects;
    }

    Span<const PointLight> Scene::point_lights() const {
        return _point_lights;
    }

    Camera &Scene::camera() {
        return _camera;
    }

    const Camera &Scene::camera() const {
        return _camera;
    }

    void Scene::set_sun(glm::vec3 direction, glm::vec3 color) {
        _sun_direction = direction;
        _sun_color = color;
    }

    void Scene::render() const {
        // Fill and bind frame data buffer
        TypedBuffer<shader::FrameData> buffer(nullptr, 1);
        {
            auto mapping = buffer.map(AccessType::WriteOnly);
            mapping[0].camera.view_proj = _camera.view_proj_matrix();
            mapping[0].point_light_count = u32(_point_lights.size());
            mapping[0].sun_color = _sun_color;
            mapping[0].sun_dir = glm::normalize(_sun_direction);
        }
        buffer.bind(BufferUsage::Uniform, 0);

        // Fill and bind lights buffer
        TypedBuffer<shader::PointLight> light_buffer(nullptr, std::max(_point_lights.size(), size_t(1)));
        {
            auto mapping = light_buffer.map(AccessType::WriteOnly);
            for (size_t i = 0; i != _point_lights.size(); ++i) {
                const auto &light = _point_lights[i];
                mapping[i] = {
                        light.position(),
                        light.radius(),
                        light.color(),
                        0.0f
                };
            }
        }
        light_buffer.bind(BufferUsage::Storage, 1);

        std::vector<SceneObject> obj1;
        std::vector<SceneObject> obj2;
        std::vector<glm::mat4> obj1_transforms;
        std::vector<glm::mat4> obj2_transforms;

        // Render every object
        for (const SceneObject &obj: _objects) {
            if (_camera.isSphereOutsideFrustum(_camera.build_frustum(), {obj.transform() * glm::vec4(obj.getMesh()->bounding_sphere.center, 1), obj
            .getMesh()
            ->bounding_sphere.radius})) {
                continue;
            }
            if (obj1.empty() || obj.getMaterial() == obj1[0].getMaterial()) {
                obj1.emplace_back(obj);
                obj1_transforms.emplace_back(obj.transform());
            } else {
                obj2.emplace_back(obj);
                obj2_transforms.emplace_back(obj.transform());
            }
//            obj.render();
        }
        obj1[0].getMaterial()->bind();
        TypedBuffer<glm::mat4>(obj1_transforms.data(), obj1_transforms.size()).bind(BufferUsage::Storage, 2);
        obj1[0].getMesh()->draw(obj1.size());
        obj2[0].getMaterial()->bind();
        TypedBuffer<glm::mat4>(obj2_transforms.data(), obj2_transforms.size()).bind(BufferUsage::Storage, 2);
        obj2[0].getMesh()->draw(obj2.size());

    }

}
