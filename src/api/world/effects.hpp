#pragma once
#ifndef SPF_EFFECTS_H
#define SPF_EFFECTS_H

#include <amogpu/amogpu.hpp>
#include <glm/glm.hpp>

namespace gpu {
    extern shape_builder shape;
    extern font_renderer font;

    void init();
    void push_back_vec(std::vector<float> &vec, float x, float y, float z);
};

namespace effects {
    extern amogpu::gpu_gl_program program_model_lighting;
    extern amogpu::gpu_gl_program program_model_instanced;

    enum objecttype {
        LIGHT
    };

    struct object {
        effects::objecttype type {effects::objecttype::LIGHT};
        int32_t index {0};

        glm::vec3 position {glm::vec3(0, 0, 0)};
        glm::vec3 kd {glm::vec3(0, 0, 0)};
        glm::vec3 ld {glm::vec3(0, 0, 0)};
        float flare {32};
    };

    struct world_object {
        std::vector<effects::object> objects {};

        bool should_update {};
        void update();
    };

    void world_render(amogpu::gpu_gl_program &program);
    void process_world_objects(amogpu::gpu_gl_program &program);
    void add_world_objects(effects::object &object);

    void load();
}

#endif