#include "model.hpp"
#include "utils/log.hpp"
#include "utils/math.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void matrix::look_at(const glm::vec3 &vec) {
    this->direction3f = vec - this->position3f;
    this->rotation4fv = glm::mat4(math::get_rotation_between(this->position3f, vec));
    this->normal3f = glm::normalize(this->direction3f);
}

void matrix::on_update() {
    this->model4fv = glm::mat4(1.0);
    this->model4fv = glm::translate(this->model4fv, this->position3f);

    if (this->enable_axis_rotation) {
        this->model4fv = glm::rotate(this->model4fv, glm::radians(this->rotation3f.x), glm::vec3(1, 0, 0));
        this->model4fv = glm::rotate(this->model4fv, glm::radians(this->rotation3f.y), glm::vec3(0, 1, 0));
        this->model4fv = glm::rotate(this->model4fv, glm::radians(this->rotation3f.z), glm::vec3(0, 0, 1));
    } else {
        this->model4fv *= this->rotation4fv;
    }

    this->model4fv = glm::scale(this->model4fv, this->scale3f);
    this->normal3fv = glm::mat3(this->model4fv);
    this->normal3fv = glm::inverseTranspose(this->normal3fv);
}

void model::create_model() {
    glGenVertexArrays(1, &this->vao);

    this->null = false;

    if (this->matrix_list.empty()) {
        this->matrix_list.emplace_back();
    }
}

void model::delete_model() {

}

void model::create_matrix(matrix mat) {
    this->matrix_list.push_back(mat);
}

void model::invoke() {
    glBindVertexArray(this->vao);
}

void model::bind(GLuint vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    this->concurrent_vbo = vbo;
}

void model::dispatch(GLuint shader_location, GLint per_index, size_t sizeof_data, void *data, GLint vertex_count) {
    glEnableVertexAttribArray(shader_location);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof_data, data, GL_STATIC_DRAW);
    glVertexAttribPointer(shader_location, per_index, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    if (vertex_count != 0) {
        this->total_vertex_count = vertex_count / per_index;
        log_message("Dispatching vertex(s): " + std::to_string(this->total_vertex_count));
    }
}

void model::revoke() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void model::on_matrix(amogpu::gpu_gl_program &program) {
    auto &model_matrix = this->matrix_list[0];

    program.setm4f("MatrixModel", &model_matrix.model4fv[0][0]);
    program.setm3f("MatrixNormal", &model_matrix.normal3fv[0][0]);
}

void model::draw() {
    if (this->null) {
        return;
    }

    switch (this->mode) {
        case model_draw_mode::INSTANCED: {
            glBindVertexArray(this->vao);
            glDrawArraysInstanced(primitive, 0, this->total_vertex_count, this->instanced_size);
            glBindVertexArray(0);
            break;
        }

        case model_draw_mode::ARRAYS: {
            glBindVertexArray(this->vao);
            glDrawArrays(primitive, 0, this->total_vertex_count);
            glBindVertexArray(0);
        }
    }
}

void model::bind(const std::string &vbo_key) {
    if (!this->vbo_map.count(vbo_key)) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        this->vbo_map[vbo_key] = vbo;
    }

    this->bind(this->vbo_map[vbo_key]);
}

void model::instanced(GLint size, GLint mat4_size, void *data) {
    glBufferData(GL_ARRAY_BUFFER, mat4_size * size, data, GL_STATIC_DRAW);
    log_message("Instanced vertex(s): " + std::to_string(size));
}

void model::dispatch_instanced(GLuint shader_location, GLint per_index, GLsizeiptr vec4_size) {
    GLint size = (GLint) vec4_size * per_index;

    glEnableVertexAttribArray(shader_location);
    glVertexAttribPointer(shader_location++, per_index, GL_FLOAT, GL_FALSE, size, (void*) 0);

    glEnableVertexAttribArray(shader_location);
    glVertexAttribPointer(shader_location++, per_index, GL_FLOAT, GL_FALSE, size, (void*) (vec4_size * 1));

    glEnableVertexAttribArray(shader_location);
    glVertexAttribPointer(shader_location++, per_index, GL_FLOAT, GL_FALSE, size, (void*) (vec4_size * 2));

    glEnableVertexAttribArray(shader_location);
    glVertexAttribPointer(shader_location, per_index, GL_FLOAT, GL_FALSE, size, (void*) (vec4_size * 3));

    shader_location -= 3;

    glVertexAttribDivisor(shader_location++, 1);
    glVertexAttribDivisor(shader_location++, 1);
    glVertexAttribDivisor(shader_location++, 1);
    glVertexAttribDivisor(shader_location, 1);
}

matrix &model::get_default_matrix() {
    return this->matrix_list[0]; // default matrix is always 0.
}
