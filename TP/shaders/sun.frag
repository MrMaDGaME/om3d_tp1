#version 450

layout(location = 0) in vec2 uv;

layout(binding = 0) uniform sampler2D albedoTexture;
layout(binding = 1) uniform sampler2D normalTexture;

uniform vec3 sun_direction;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 albedo = texelFetch(albedoTexture, ivec2(gl_FragCoord.xy), 0).rgb;
    vec3 normal = texture(normalTexture, uv).rgb;

    float diffuse = (dot(normal, -sun_direction));
    vec3 color = diffuse * albedo * 0.2;

    fragColor = vec4(color, 1.0);
}
