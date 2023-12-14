#version 450

#define MAX_LIGHTS 100

layout(location = 0) in vec2 uv;

layout(binding = 0) uniform sampler2D albedoTexture;
layout(binding = 1) uniform sampler2D normalTexture;
layout(binding = 2) uniform sampler2D depthTexture;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform mat4 view_proj;

layout(location = 0) out vec4 fragColor;

vec3 unproject(vec2 uv, float depth, mat4 inv_viewproj) {
    const vec3 ndc = vec3(uv * 2.0 - vec2(1.0), depth);
    const vec4 p = inv_viewproj * vec4(ndc, 1.0);
    return p.xyz / p.w;
}

void main() {
    vec3 albedo = texelFetch(albedoTexture, ivec2(gl_FragCoord.xy), 0).rgb;
    vec3 normal = normalize(texelFetch(normalTexture, ivec2(gl_FragCoord.xy), 0).xyz);
    float depth = texelFetch(depthTexture, ivec2(gl_FragCoord.xy), 0).r;

    vec3 fragPos = unproject(uv, depth, inverse(view_proj));
    vec3 color = vec3(0.0);

    float distanceToLight = distance(lightPosition, fragPos);

    if (distanceToLight < 10.0) {
        vec3 lightDir = normalize(lightPosition - fragPos);
        float lambertian = abs(dot(normal, lightDir));
        color = albedo * lambertian * (1.0 / pow(distanceToLight, 3.0)) * lightColor;
    }

    fragColor = vec4(color, 1.0); 
}