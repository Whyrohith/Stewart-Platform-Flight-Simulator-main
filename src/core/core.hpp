#pragma once
#ifndef SPF_CORE_H
#define SPF_CORE_H

#include "utils/timing.hpp"
#include "api/world/effects.hpp"
#include <SDL2/SDL.h>
#include "api/context/context.hpp"
#include "api/service/module_manager.hpp"
#include "api/service/widget_manager.hpp"
#include "api/world/camera.hpp"

class activy_core {
public:
    SDL_Window* sdl_window {};
    SDL_GLContext sdl_gl_context {};

    uint64_t elapsed_frame  {};
    uint64_t fps {};
    uint64_t interval_fps = {};

    module_manager module_service {};
    widget_manager widget_service {};

    context* activy_context {nullptr};
    activy_camera camera {};
    effects::world_object world_object {};

    bool running {true};

    timing::clock fps_clock {};
    timing::clock loop_clock {};

    void make_current(context* raw_context);

    void on_event(SDL_Event &sdl_event);
    void on_update();
    void on_render();

    void init();
    void shutdown();
    void run();
};

#endif