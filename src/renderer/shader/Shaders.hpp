/// @file    Shaders.hpp
/// @brief   GLSL shader sources (OpenGL 4.1 Core compatible).
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

namespace cad::shaders {

// Cross-platform: GLSL 410 core works on both macOS and Windows
inline constexpr const char* PbrVertexShader = R"(
#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uViewProjection;
uniform mat3 uNormalMatrix;

out vec3 vWorldPos;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {
    vec4 worldPos = uModel * vec4(aPosition, 1.0);
    vWorldPos = worldPos.xyz;
    vNormal = normalize(uNormalMatrix * aNormal);
    vTexCoord = aTexCoord;
    gl_Position = uViewProjection * worldPos;
}
)";

inline constexpr const char* PbrFragmentShader = R"(
#version 410 core

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 uCameraPos;
uniform vec3 uBaseColor;
uniform float uMetallic;
uniform float uRoughness;
uniform float uShininess;
uniform float uSelected;

out vec4 fragColor;

const vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float ambientStrength = 0.3;

void main() {
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uCameraPos - vWorldPos);
    
    // Blinn-Phong lighting
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular: shininess controls intensity, roughness controls spread
    vec3 halfDir = normalize(lightDir + viewDir);
    float specPower = mix(8.0, 128.0, 1.0 - uRoughness);
    float spec = pow(max(dot(normal, halfDir), 0.0), specPower) * uShininess;
    
    vec3 ambient = ambientStrength * uBaseColor;
    vec3 diffuse = diff * uBaseColor * lightColor;
    vec3 specular = spec * mix(vec3(0.04), uBaseColor, uMetallic) * lightColor;
    
    vec3 color = ambient + diffuse + specular;
    
    // Selection highlight
    if (uSelected > 0.5) {
        color = mix(color, vec3(1.0, 0.8, 0.2), 0.3);
    }
    
    // Simple tone mapping
    color = color / (color + vec3(1.0));
    
    // Gamma correction
    color = pow(color, vec3(1.0/2.2));
    
    fragColor = vec4(color, 1.0);
}
)";

// Grid shader for visual reference
inline constexpr const char* GridVertexShader = R"(
#version 410 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uViewProjection;

out vec3 vWorldPos;

void main() {
    vWorldPos = aPosition;
    gl_Position = uViewProjection * vec4(aPosition, 1.0);
}
)";

inline constexpr const char* GridFragmentShader = R"(
#version 410 core

in vec3 vWorldPos;

out vec4 fragColor;

void main() {
    // Create grid pattern
    vec2 grid = abs(fract(vWorldPos.xz - 0.5) - 0.5);
    float line = min(grid.x, grid.y);
    float gridIntensity = 1.0 - smoothstep(0.0, 0.05, line);
    
    // Fade with distance
    float dist = length(vWorldPos.xz);
    float fade = 1.0 - smoothstep(10.0, 50.0, dist);
    
    // Axis highlights
    float axisX = 1.0 - smoothstep(0.0, 0.1, abs(vWorldPos.z));
    float axisZ = 1.0 - smoothstep(0.0, 0.1, abs(vWorldPos.x));
    
    vec3 color = vec3(0.3) * gridIntensity;
    color = mix(color, vec3(1.0, 0.2, 0.2), axisX * 0.5);
    color = mix(color, vec3(0.2, 0.2, 1.0), axisZ * 0.5);
    
    float alpha = max(gridIntensity, max(axisX, axisZ)) * fade * 0.5;
    
    fragColor = vec4(color, alpha);
}
)";

} // namespace cad::shaders
