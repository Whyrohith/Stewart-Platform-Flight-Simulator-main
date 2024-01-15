#pragma once
#ifndef SPF_CONTEXT_H
#define SPF_CONTEXT_H

#include "api/feature/feature.hpp"

class context {
public:
    std::string tag {};
    std::string description {};

    virtual void on_create();
    virtual void on_destroy();

    virtual void on_event(SDL_Event &sdl_event);
    virtual void on_update();
    virtual void on_render();
};

#endif