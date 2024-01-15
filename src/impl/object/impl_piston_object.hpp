#pragma once
#ifndef SPF_IMPL_OBJECT_H
#define SPF_IMPL_OBJECT_H

#include "api/feature/feature.hpp"
#include "api/world/effects.hpp"
#include "api/model/model.hpp"
#include "api/ui/ui.hpp"

class impl_piston_object : public feature {
public:
    model model_bar {};
    model model_base {};

    ui::number* force {};
    ui::number* color_red {};
    ui::number* color_green {};
    ui::number* color_blue {};

    float force_amount_height {};
    float base_force_height {};

    impl_piston_object(const std::string &tag_of) {
        this->tag = tag_of;
    }

    void set_angle(const glm::vec3 &universal_joint_pos, const glm::vec3 &spherical_joint_pos);
    void set_size(const glm::vec3 &universal_joint_pos, const glm::vec3 &spherical_joint_pos);

    void on_reload() override;
    void on_matrix();

    void on_enable() override;
    void on_disable() override;
    void on_event(SDL_Event &sdl_event) override;
    void on_render() override;
    void on_update() override;
};

#endif