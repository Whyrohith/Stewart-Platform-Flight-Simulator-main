#include "impl_stewart_platform_object.hpp"
#include "api/world/effects.hpp"
#include "core/client.hpp"
#include "utils/log.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

glm::vec3 impl_stewart_platform_object::get_pos() {
    return this->model_plane.get_default_matrix().position3f;
}

void impl_stewart_platform_object::on_event(SDL_Event &sdl_event) {
    feature::on_event(sdl_event);
}

void impl_stewart_platform_object::on_update() {
    feature::on_update();
    this->on_matrix();
}

void impl_stewart_platform_object::on_render() {
    feature::on_render();

    effects::world_render(effects::program_model_lighting);
    effects::program_model_lighting.set3f("Color", &glm::vec3(this->color_red->get_x() / 255, this->color_green->get_x() / 255, this->color_blue->get_x() / 255)[0]);

    this->model_plane.on_matrix(effects::program_model_lighting);
    this->model_plane.draw();
}

void impl_stewart_platform_object::on_matrix() {
    this->model_plane.get_default_matrix().on_update();

    this->detail_angles->set_x(glm::degrees(this->model_plane.get_default_matrix().rotation3f.x));
    this->detail_angles->set_y(glm::degrees(this->model_plane.get_default_matrix().rotation3f.z));
}

glm::vec3 impl_stewart_platform_object::get_vertex(int32_t index) {
    return this->model_plane.get_default_matrix().model4fv * glm::vec4(this->model_plane.matrix_list.at(index + 1).position3f, 1.0f);
}

void impl_stewart_platform_object::set_scale(const glm::vec3 &vec) {
    auto &default_matrix = this->model_plane.get_default_matrix();
    default_matrix.scale3f = vec;
}

void impl_stewart_platform_object::set_pos(const glm::vec3 &vec) {
    auto &default_matrix = this->model_plane.get_default_matrix();
    default_matrix.position3f = vec;
}

void impl_stewart_platform_object::on_reload() {
    if (this->model_plane.null) {
        this->model_plane.create_model();
        this->model_plane.get_default_matrix().enable_axis_rotation = true;
    }

    float mesh[] = {
            // Vertex 1.
            -0.5f, 0.0f, -1.0f,
            0.0f, 0.0f, 0.0f,
            0.5f, 0.0f, -1.0f,

            // Vertex 2.
            0.5f, 0.0f, -1.0f,
            0.0f, 0.0f, 0.0f,
            0.75f, 0.0f, -0.25f,

            // Vertex 3.
            0.75f, 0.0f, -0.25f,
            0.0f, 0.0f, 0.0f,
            0.25f, 0.0f, 1.0f,

            // Vertex 4.
            0.25f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,
            -0.25f, 0.0f, 1.0f,

            // Vertex 5.
            -0.25f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,
            -0.75f, 0.0f, -0.25f,

            // Vertex 6.
            -0.75f, 0.0f, -0.25f,
            0.0f, 0.0f, 0.0f,
            -0.5f, 0.0f, -1.0f
    };

    float mesh_normalised[] = {
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    int32_t array_size {9 * 6};
    int32_t next_iteration {};
    int32_t matrix_size {(int32_t) this->model_plane.matrix_list.size() - 1}; // ignore the default matrix.

    matrix current_matrix {};
    glm::vec3 vertex {};
    int32_t count {1};

    for (int32_t it = 1; it < array_size + 1; it += 9) {
        it--;

        if (it > array_size) {
            break;
        }

        next_iteration = it++;

        vertex.x = mesh[next_iteration++];
        vertex.y = mesh[next_iteration++];
        vertex.z = mesh[next_iteration++];

        if (count > matrix_size) {
            current_matrix.position3f = vertex;
            this->model_plane.create_matrix(current_matrix);
        } else {
            this->model_plane.matrix_list.at(count).position3f = vertex;
        }

        count++;

        if (count >= 7) { // huh, just whatever
            break;
        }
    }

    this->model_plane.invoke();
    this->model_plane.bind("vertex");
    this->model_plane.dispatch(0, 3, array_size, mesh, array_size);

    this->model_plane.bind("normals");
    this->model_plane.dispatch(1, 3, array_size, &mesh_normalised);
    this->model_plane.revoke();

    this->model_plane.primitive = GL_TRIANGLES;
}

void impl_stewart_platform_object::on_enable() {
    feature::on_enable();
}

void impl_stewart_platform_object::on_disable() {
    feature::on_disable();
}
