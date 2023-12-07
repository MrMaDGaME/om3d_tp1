
#version 450

#include "utils.glsl"

layout(location = 0) in vec2 in_uv;

layout(location = 0) out vec4 outFragColor;

layout(binding = 0) uniform sampler2D albedoTexture;
layout(binding = 1) uniform sampler2D normalTexture;
layout(binding = 2) uniform sampler2D depthTexture;

uniform uint displayMode;

const uint NONE = 0;
const uint ALBEDO = 1;
const uint NORMALS = 2;
const uint DEPTH = 3;

void main()
{
    const ivec2 texCoords = ivec2(gl_FragCoord.xy);
    vec3 color = vec3(0.25, 0.25, 0.25);
    if (displayMode == ALBEDO) {
        color = texelFetch(albedoTexture, texCoords, 0).rgb;
    } else if (displayMode == NORMALS) {
        color = texelFetch(normalTexture, texCoords, 0).rgb;
    } else if (displayMode == DEPTH) {
        float depth = texelFetch(depthTexture, texCoords, 0).r;
        depth = pow(depth, 0.35);
        color = vec3(depth);
    }
    outFragColor = vec4(color, 1.0);
}