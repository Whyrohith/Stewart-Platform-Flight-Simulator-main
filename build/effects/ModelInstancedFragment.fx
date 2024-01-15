#version 450 core

layout (location = 0) out vec4 FragColor;
in vec3 VertexColor;

void main() {
    FragColor = vec4(VertexColor, 1.0f);
}