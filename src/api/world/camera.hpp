#pragma once
#ifndef SPF_CAMERA_H
#define SPF_CAMERA_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class activy_camera {
public:
    glm::vec3 position {};
    glm::vec3 front {};
    glm::vec3 up {};
    glm::vec3 right {};
    glm::vec3 world_up {};

    float yaw {};
    float pitch {};

    float fov {60.0f};
    float mouse_sensitivity {0.1f};
    bool mouse_locked {false};

    float previous_mx;
    float previous_my;

    glm::mat4 matrix_camera_view {};
    glm::mat4 matrix_projection_perspective {};

    void set_camera_focused(bool val);

    void on_init();
    void on_event(SDL_Event &sdl_event);
    void on_update();
    void on_update_matrix();

    glm::mat4 &get_camera_view_matrix();
};

#endif