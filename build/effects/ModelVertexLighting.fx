#version 450 core

layout (location = 0) in vec3 VertexMeshData;
layout (location = 1) in vec3 NormalMeshData;

out vec3 Vertex;
out vec3 Normal;
out vec3 VertexColor;

uniform mat4 MatrixProjectionPerspective;
uniform mat4 MatrixCameraView;
uniform mat4 MatrixModel;
uniform mat3 MatrixNormal;

void main() {
    vec4 M = MatrixModel * vec4(VertexMeshData, 1.0f);
    gl_Position = MatrixProjectionPerspective * MatrixCameraView * M;

    Vertex = M.xyz;
    Normal = normalize(MatrixNormal * NormalMeshData);
}