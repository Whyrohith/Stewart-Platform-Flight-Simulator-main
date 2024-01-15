#version 450 core

layout (location = 0) out vec4 FragColor;

in vec3 Vertex;
in vec3 Normal;
in vec3 VertexColor;

uniform bool EnableLighting;
uniform vec3 Color;
uniform vec3 Camera;

uniform struct {
    vec3 SpotPos;
    vec3 Kd;
    float Flare;
} Lighting;

vec3 FinalColor;

void main() {
    FinalColor = Color;

    if (EnableLighting) {
        FinalColor = 0.1f * Lighting.Kd;

        vec3 S = normalize(Lighting.SpotPos - Vertex);
        vec3 Diffuse = max(dot(S, Normal), 0.0f) * Lighting.Kd;

        vec3 ViewDir = normalize(Camera - Vertex);
        vec3 ReflectDir = reflect(-S, Normal);

        float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0f), Lighting.Flare);
        vec3 Specular = 0.5f * Spec * Lighting.Kd;

        FinalColor = (FinalColor + Diffuse + Specular) * Color;
    }

    FragColor = vec4(FinalColor, 1.0f);
}