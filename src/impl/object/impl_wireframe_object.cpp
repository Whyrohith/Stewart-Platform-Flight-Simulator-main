#include "impl_wireframe_object.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "api/world/effects.hpp"
#include "utils/log.hpp"

void impl_wireframe_object::on_reload() {
    if (this->model_element.null) {
        this->model_element.create_model();
    }

    std::vector<float> mesh {};

    float minx = 0;
    float minz = 0;
    float maxx = 1;
    float maxz = 1;

    // AABB
    gpu::push_back_vec(mesh, minx, 0, minz);
    gpu::push_back_vec(mesh, minx, 0, minz + maxz);
    gpu::push_back_vec(mesh, minx, 0, minz + maxz);
    gpu::push_back_vec(mesh, minx + maxx, 0, minz + maxz);
    gpu::push_back_vec(mesh, minx + maxx, 0, minz + maxz);
    gpu::push_back_vec(mesh, minx + maxx, 0, minz);
    gpu::push_back_vec(mesh, minx + maxx, 0, minz);
    gpu::push_back_vec(mesh, minx, 0, minz);

    this->model_element.invoke();
    this->model_element.bind("vertex");
    this->model_element.dispatch(0, 3, mesh.size(), &mesh[0], (int32_t) mesh.size());

    mesh.clear();

    for (int32_t repeat_amount = 0; repeat_amount < 8; repeat_amount++) {
        gpu::push_back_vec(mesh, 0.2f, 0.2f, 0.2f);
    }

    this->model_element.bind("color");
    this->model_element.dispatch(1, 3, mesh.size(), &mesh[0]);

    mesh.clear();

    effects::program_model_instanced.use();
    int32_t count {0};
    glm::mat4 model_wireframe {};
    int32_t it = 0;

    for (int32_t x = 0; x < this->range; x++) {
        for (int32_t z = 0; z < this->range; z++) {
            model_wireframe = glm::mat4(1);
            model_wireframe = glm::translate(model_wireframe, glm::vec3(x - (range / 2), -0.1f, z - (range / 2)));

            auto v = glm::value_ptr(model_wireframe);
            mesh.insert(mesh.end(), &v[0], &v[16]);

            it++;
        }
    }

    this->model_element.bind("instanced");
    this->model_element.instanced(it, sizeof(glm::mat4), &mesh[0]);
    this->model_element.dispatch_instanced(2, 4, sizeof(glm::vec4));
    this->model_element.revoke();

    this->model_element.primitive = GL_LINES;
    this->model_element.mode = model_draw_mode::INSTANCED;
    this->model_element.instanced_size = it;
}

void impl_wireframe_object::on_enable() {
    feature::on_enable();
    this->range = 64;
}

void impl_wireframe_object::on_disable() {
    feature::on_disable();
}

void impl_wireframe_object::on_event(SDL_Event &sdl_event) {
    feature::on_event(sdl_event);
}

void impl_wireframe_object::on_render() {
    feature::on_render();
    effects::world_render(effects::program_model_instanced);

    this->model_element.draw();
}

void impl_wireframe_object::on_update() {
    feature::on_update();
}
