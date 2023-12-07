#version 450

#include "utils.glsl"

// fragment shader of the main lighting pass

// #define DEBUG_NORMAL

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;

layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_color;
layout(location = 3) in vec3 in_position;
layout(location = 4) in vec3 in_tangent;
layout(location = 5) in vec3 in_bitangent;

layout(binding = 0) uniform sampler2D in_texture;
layout(binding = 1) uniform sampler2D in_normal_texture;

layout(binding = 0) uniform Data {
    FrameData frame;
};

layout(binding = 1) buffer PointLights {
    PointLight point_lights[];
};

const vec3 ambient = vec3(0.0);

void main()
{
    #ifdef NORMAL_MAPPED
        const vec3 normal_map = unpack_normal_map(texture(in_normal_texture, in_uv).xy);
        out_normal = vec4(normal_map.x * in_tangent + normal_map.y * in_bitangent + normal_map.z * in_normal, 1.0) * 0.5 + 0.5;
    #else
        out_normal = vec4(in_normal, 1.0) * 0.5 + 0.5;
    #endif
        vec4 color = vec4(in_color, 1.0);
    #ifdef TEXTURED
        color *= texture(in_texture, in_uv);
    #endif
    #ifdef DEBUG_NORMAL
        color = vec4(normal, 1.0) * 0.5 + 0.5;
    #endif
        out_color = color;
}

