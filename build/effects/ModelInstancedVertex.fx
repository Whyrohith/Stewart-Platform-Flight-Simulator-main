#version 450 core

layout (location = 0) in vec3 VertexData;
layout (location = 1) in vec3 VertexColorData;
layout (location = 2) in mat4 MatrixModelInstanceData;

uniform mat4 MatrixCameraView;
uniform mat4 MatrixProjectionPerspective;

out vec3 VertexColor;

void main() {
    gl_Position = MatrixProjectionPerspective * MatrixCameraView * MatrixModelInstanceData * vec4(VertexData, 1.0f);
    VertexColor = VertexColorData;
}