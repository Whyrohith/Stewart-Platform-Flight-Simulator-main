#pragma once
#ifndef SPF_IMPL_STEWART_PLATFORM_OBJECT_H
#define SPF_IMPL_STEWART_PLATFORM_OBJECT_H

#include "api/model/model.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <SDL2/SDL.h>
#include "api/feature/feature.hpp"
#include "api/ui/ui.hpp"

class impl_stewart_platform_object : public feature {
public:
    model model_plane {};

    impl_stewart_platform_object(const std::string &tag_of) {
        this->tag = tag_of;
        this->model_plane.create_matrix({});
    }

    ui::number* color_red {};
    ui::number* color_green {};
    ui::number* color_blue {};
    ui::number* detail_angles {};

    void set_scale(const glm::vec3 &vec);
    void set_pos(const glm::vec3 &vec);
    void on_matrix();

    glm::vec3 get_pos();
    glm::vec3 get_vertex(int32_t index);

    void on_reload() override;
    void on_enable() override;
    void on_disable() override;
    void on_event(SDL_Event &sdl_event) override;
    void on_render() override;
    void on_update() override;
};

#endif