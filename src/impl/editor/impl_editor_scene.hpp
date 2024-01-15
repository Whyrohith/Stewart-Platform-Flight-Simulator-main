#pragma once
#ifndef SPF_IMPL_EDITOR_SCENE_H
#define SPF_IMPL_EDITOR_SCENE_H

#include "api/feature/feature.hpp"
#include "impl/object/impl_stewart_platform_object.hpp"
#include "impl/object/impl_wireframe_object.hpp"
#include "impl/object/impl_piston_object.hpp"
#include "api/world/effects.hpp"
#include "api/ui/ui.hpp"

class impl_editor_scene : public feature {
public:
    std::vector<feature*> object_list;
    dynamic_batching batch;

    glm::vec3 camera_velocity {};
    glm::vec3 camera_direction {};
    glm::vec3 dpad_velocity {};

    impl_piston_object* the_piston0 {};
    impl_piston_object* the_piston1 {};
    impl_piston_object* the_piston2 {};
    impl_piston_object* the_piston3 {}; // uwu
    impl_piston_object* the_piston4 {};
    impl_piston_object* the_piston5 {};

    impl_stewart_platform_object* inferior {};
    impl_stewart_platform_object* superior {};

    float camera_speed {0.7873f};

    bool enable_camera_movement {true};
    bool should_update_platform {true};

    ui::number* setting_max_roll {};
    ui::number* setting_max_yaw {};
    ui::number* setting_dpad_speed_factor {};

    impl_editor_scene();
    ~impl_editor_scene();

    void apply_movement_physics();
    void create_basic_models();

    feature* get_object_by_tag(const std::string &tag);

    void on_enable() override;
    void on_disable() override;
    void on_event(SDL_Event &sdl_event) override;
    void on_update() override;
    void on_render() override;
};

#endif