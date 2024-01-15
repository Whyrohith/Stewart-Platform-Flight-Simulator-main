#pragma once
#ifndef SPF_INPUT_H
#define SPF_INPUT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <map>
#include <glm/glm.hpp>

namespace input {
    extern glm::vec4 mouse;
    extern glm::vec2 mousewheel;

    extern float deltatime;
    extern std::map<std::string, bool> event;

    struct screen {
        static float width;
        static float height;
    };

    void update(SDL_Event &sdl_event);
    void key_movement(glm::vec3 &direction);
    void reset_mouse();
}

#endif