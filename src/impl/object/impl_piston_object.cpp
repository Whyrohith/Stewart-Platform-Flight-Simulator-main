#include "impl_piston_object.hpp"
#include "utils/log.hpp"
#include "utils/math.hpp"

void impl_piston_object::on_enable() {
    feature::on_enable();
}

void impl_piston_object::on_disable() {
    feature::on_disable();
}

void impl_piston_object::on_event(SDL_Event &sdl_event) {
    feature::on_event(sdl_event);
}

void impl_piston_object::on_render() {
    feature::on_render();

    effects::world_render(effects::program_model_lighting);
    effects::program_model_lighting.set3f("Color", &glm::vec3(this->color_red->get_x() / 255, this->color_green->get_x() / 255, this->color_blue->get_x() / 255)[0]);

    this->model_bar.on_matrix(effects::program_model_lighting);
    this->model_bar.draw();

    effects::program_model_lighting.set3f("Color", &glm::vec3(this->color_red->get_x() / 255, this->color_green->get_x() / 255, this->color_blue->get_x() / 255)[0]);
    this->model_base.on_matrix(effects::program_model_lighting);
    this->model_base.draw();

    glUseProgram(0);
}

void impl_piston_object::set_angle(const glm::vec3 &universal_joint_pos, const glm::vec3 &spherical_joint_pos) {
    auto &default_matrix = this->model_base.get_default_matrix();

    default_matrix.position3f = universal_joint_pos;
    default_matrix.look_at(spherical_joint_pos);
}

void impl_piston_object::on_update() {
    feature::on_update();
    this->on_matrix();
}

void impl_piston_object::on_matrix() {
    auto &base_default_matrix = this->model_base.get_default_matrix();

    const glm::vec3 curr_scale = base_default_matrix.scale3f;
    const glm::vec3 prev_scale = base_default_matrix.scale3f / glm::vec3(1, 2, 1);

    this->base_force_height = prev_scale.y;
    this->force_amount_height = (this->force->get_maximum() - this->force->get_x()) * prev_scale.y;

    const glm::vec3 amount_scale = glm::vec3(curr_scale.x, curr_scale.y - this->force_amount_height, curr_scale.z);
    base_default_matrix.scale3f = prev_scale;

    auto &bar_default_matrix = this->model_bar.get_default_matrix();
    bar_default_matrix = base_default_matrix;
    bar_default_matrix.scale3f = amount_scale;

    base_default_matrix.on_update();
    bar_default_matrix.on_update();

    base_default_matrix.scale3f = curr_scale;
}

void impl_piston_object::on_reload() {
    if (this->model_base.null) {
        this->model_base.create_model();
    }

    if (this->model_bar.null) {
        this->model_bar.create_model();
    }

    float mesh_cube[] = {
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f
    };

    float nm_faces_mesh[] = {
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f
    };

    this->model_base.invoke();
    this->model_base.bind("vertex");
    this->model_base.dispatch(0, 3, 3 * 36, mesh_cube, 36 * 3);

    this->model_base.bind("normals");
    this->model_base.dispatch(1, 3, 3 * 36, nm_faces_mesh);
    this->model_base.revoke();

    this->model_bar.invoke();
    this->model_bar.bind("vertex");
    this->model_bar.dispatch(0, 3, 3 * 36, mesh_cube, 36 * 3);

    this->model_bar.bind("normals");
    this->model_bar.dispatch(1, 3, 3 * 36, nm_faces_mesh);
    this->model_bar.revoke();
}

void impl_piston_object::set_size(const glm::vec3 &universal_joint_pos, const glm::vec3 &spherical_joint_pos) {
    glm::vec3 diff = spherical_joint_pos - universal_joint_pos;
    this->model_base.get_default_matrix().scale3f = glm::vec3(0.25f, glm::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z), 0.25f);
}