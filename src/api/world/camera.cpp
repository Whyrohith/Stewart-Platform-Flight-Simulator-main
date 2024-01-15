#include "camera.hpp"
#include "utils/input.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "utils/log.hpp"
#include "core/client.hpp"

void activy_camera::on_update() {
    glm::vec3 vec_front;

    vec_front.x = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
    vec_front.y = glm::sin(glm::radians(this->pitch));
    vec_front.z = glm::sin(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));

    this->front = glm::normalize(vec_front);
    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

glm::mat4 &activy_camera::get_camera_view_matrix() {
    this->matrix_camera_view = glm::lookAt(this->position, this->position + this->front, this->up);
    return this->matrix_camera_view;
}

void activy_camera::on_update_matrix() {
    this->matrix_projection_perspective = glm::perspective(glm::radians(this->fov), (input::screen::width / input::screen::height), 0.1f, 100.0f);
}

void activy_camera::on_event(SDL_Event &sdl_event) {
    switch (sdl_event.type) {
        case SDL_MOUSEMOTION: {
            if (this->mouse_locked) {
                float diff_mx = static_cast<float>(sdl_event.motion.xrel) * this->mouse_sensitivity;
                float diff_my = static_cast<float>(sdl_event.motion.yrel) * this->mouse_sensitivity;

                this->yaw += diff_mx;
                this->pitch -= diff_my;

                if (this->pitch > 89.0f) {
                    this->pitch = 89.9f;
                }

                if (this->pitch < -89.0f) {
                    this->pitch = -89.0f;
                }

                this->on_update();
            }

            break;
        }
    }
}

void activy_camera::set_camera_focused(bool val) {
    if (client::core.widget_service.previous_event_down_widget_id != 0 || client::core.widget_service.focused_widget_id != 0) {
        val = false;
    }

    this->mouse_locked = val;
}

void activy_camera::on_init() {
    this->position = glm::vec3(0, 0, 0);
    this->front = glm::vec3(0, 0, -1);
    this->up = glm::vec3(0, 0, 0);
    this->right = glm::vec3(0, 0, 0);
    this->world_up = glm::vec3(0, 1, 0);

    this->on_update();
}
