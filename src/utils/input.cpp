#include "input.hpp"
#include "core/client.hpp"
#include "log.hpp"

float input::screen::width {};
float input::screen::height {};
float input::deltatime {};
glm::vec4 input::mouse {};
glm::vec2 input::mousewheel {};
std::map<std::string, bool> input::event {};

void input::update(SDL_Event &sdl_event) {
    input::event["Mouse Scroll Up"] = false;
    input::event["Mouse Scroll Down"] = false;
    input::event["Mouse Scroll Left"] = false;
    input::event["Mouse Scroll Right"] = false;
    input::event["Mouse Motion"] = false;
    input::event["Mouse1 Up"] = false;
    input::event["Mouse2 Up"] = false;
    input::event["Mouse3 Up"] = false;
    input::event["Mouse Up"] = false;
    input::event["Mouse Down"] = false;
    input::event["Window Size"] = false;

    switch (sdl_event.type) {
        case SDL_WINDOWEVENT: {
            switch (sdl_event.window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED: {
                    input::screen::width = static_cast<float>(static_cast<int32_t>(sdl_event.window.data1));
                    input::screen::height = static_cast<float>(static_cast<int32_t>(sdl_event.window.data2));
                    input::event["Window Size"] = true;

                    break;
                }
            }

            break;
        }

        case SDL_KEYDOWN: {
            input::event[SDL_GetKeyName(sdl_event.key.keysym.sym)] = true;
            break;
        }

        case SDL_KEYUP: {
            input::event[SDL_GetKeyName(sdl_event.key.keysym.sym)] = false;
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            input::mouse.x = static_cast<float>(sdl_event.button.x);
            input::mouse.y = static_cast<float>(sdl_event.button.y);
            input::event["Mouse Down"] = true;

            switch (sdl_event.button.button) {
                case SDL_BUTTON_LEFT: {
                    input::event["Mouse1"] = true;
                    break;
                }

                case SDL_BUTTON_MIDDLE: {
                    input::event["Mouse2"] = true;
                    break;
                }

                case SDL_BUTTON_RIGHT: {
                    input::event["Mouse3"] = true;
                    break;
                }
            }

            break;
        }

        case SDL_MOUSEWHEEL: {
            input::event["Mouse Scroll Up"] = sdl_event.wheel.y > 0;
            input::event["Mouse Scroll Down"] = sdl_event.wheel.y < 0;
            input::event["Mouse Scroll Left"] = sdl_event.wheel.x < 0;
            input::event["Mouse Scroll Right"] = sdl_event.wheel.x > 0;

            input::mousewheel.x = static_cast<float>(sdl_event.wheel.x);
            input::mousewheel.y = static_cast<float>(sdl_event.wheel.y);

            break;
        }

        case SDL_MOUSEBUTTONUP: {
            input::mouse.x = static_cast<float>(sdl_event.button.x);
            input::mouse.y = static_cast<float>(sdl_event.button.y);
            input::event["Mouse Up"] = true;

            switch (sdl_event.button.button) {
                case SDL_BUTTON_LEFT: {
                    input::event["Mouse1"] = false;
                    input::event["Mouse1 Up"] = true;
                    break;
                }

                case SDL_BUTTON_MIDDLE: {
                    input::event["Mouse2"] = false;
                    input::event["Mouse2 Up"] = true;
                    break;
                }

                case SDL_BUTTON_RIGHT: {
                    input::event["Mouse3"] = false;
                    input::event["Mouse3 Up"] = true;
                    break;
                }
            }

            break;
        }

        case SDL_MOUSEMOTION: {
            input::event["Mouse Motion"] = true;
            input::mouse.x = static_cast<float>(sdl_event.motion.x);
            input::mouse.y = static_cast<float>(sdl_event.motion.y);
            input::mouse.z = static_cast<float>(sdl_event.motion.xrel);
            input::mouse.w = static_cast<float>(sdl_event.motion.yrel);
            break;
        }
    }
}

void input::key_movement(glm::vec3 &direction) {
    if (input::event["W"]) {
        direction.z = 1.0f;
    }

    if (input::event["S"]) {
        direction.z = -1.0f;
    }

    if (input::event["A"]) {
        direction.x = 1.0f;
    }

    if (input::event["D"]) {
        direction.x = -1.0f;
    }
}

void input::reset_mouse() {
    SDL_WarpMouseInWindow(client::core.sdl_window, static_cast<int32_t>(input::screen::width / 2), static_cast<int32_t>(input::screen::height / 2));
}
