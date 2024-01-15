#pragma once
#ifndef SPF_MODEL_H
#define SPF_MODEL_H

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <amogpu/amogpu.hpp>

struct matrix {
    glm::mat4 model4fv {glm::mat4(1)};
    glm::mat3 normal3fv {glm::mat3(1)};
    glm::mat4 rotation4fv {glm::mat4(1)};

    glm::vec3 rotation3f {};
    glm::vec3 position3f {};
    glm::vec3 scale3f {};
    glm::vec3 normal3f {};
    glm::vec3 direction3f {};

    bool enable_axis_rotation {};

    void look_at(const glm::vec3 &vec);
    void on_update();
};

enum model_draw_mode {
    ARRAYS, INSTANCED
};

class model {
protected:
    std::map<std::string, GLuint> vbo_map {};

    GLuint vao {};
    GLuint concurrent_vbo {};
    GLint total_vertex_count {};
public:
    bool null {true};
    model_draw_mode mode {};

    GLuint primitive {GL_TRIANGLES};
    GLint instanced_size {};
    std::vector<matrix> matrix_list {};

    void create_model();
    void delete_model();
    void create_matrix(matrix mat);
    matrix &get_default_matrix();

    void invoke();
    void bind(GLuint vbo);
    void bind(const std::string &vbo_key);
    void dispatch(GLuint shader_location, GLint per_index, size_t sizeof_data, void* data, GLint vertex_count = 0);
    void instanced(GLint size, GLint mat4_size, void* data);
    void dispatch_instanced(GLuint shader_location, GLint per_index, GLsizeiptr vec4_size);
    void revoke();

    void on_matrix(amogpu::gpu_gl_program &program);
    void draw();
};

#endif