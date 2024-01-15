#include "effects.hpp"
#include "core/client.hpp"
#include "utils/log.hpp"

amogpu::gpu_gl_program effects::program_model_lighting {};
amogpu::gpu_gl_program effects::program_model_instanced {};

font_renderer gpu::font {};
shape_builder gpu::shape {};

void effects::load() {
    amogpu::create_program(effects::program_model_lighting, (relative_path + "effects/ModelVertexLighting.fx").c_str(), (relative_path + "effects/ModelFragmentLighting.fx").c_str());
    amogpu::create_program(effects::program_model_instanced, (relative_path + "effects/ModelInstancedVertex.fx").c_str(), (relative_path + "effects/ModelInstancedFragment.fx").c_str());
}

void effects::world_render(amogpu::gpu_gl_program &program) {
    program.use();
    program.setm4f("MatrixProjectionPerspective", &client::core.camera.matrix_projection_perspective[0][0]);
    program.setm4f("MatrixCameraView", &client::core.camera.get_camera_view_matrix()[0][0]);
}

void effects::process_world_objects(amogpu::gpu_gl_program &program) {
    program.set3f("Camera", &client::core.camera.position[0]);

    for (effects::object &objects : client::core.world_object.objects) {
        switch (objects.type) {
            case effects::objecttype::LIGHT: {
                program.setb("EnableLighting", true);
                program.set3f("Lighting.SpotPos", &objects.position[0]);
                program.set3f("Lighting.Kd", &objects.kd[0]);
                program.setf("Lighting.Flare", objects.flare);
                break;
            }
        }
    }
}

void effects::add_world_objects(effects::object &object) {
    client::core.world_object.objects.push_back(object);
    client::core.world_object.should_update = true;
}

void gpu::init() {
    gpu::font.load("data/JetBrainsMono-Light.ttf", 18);
    gpu::font.from(amogpu::invoked);
}

void gpu::push_back_vec(std::vector<float> &vec, float x, float y, float z) {
    vec.push_back(x);
    vec.push_back(y);
    vec.push_back(z);
}

void effects::world_object::update() {
    if (this->should_update) {
        effects::program_model_lighting.use();
        effects::process_world_objects(effects::program_model_lighting);
        effects::program_model_lighting.end();

        this->should_update = false;
    }
}
