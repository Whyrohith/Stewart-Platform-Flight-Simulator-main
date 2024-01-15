#pragma once
#ifndef SPF_FEATURE_H
#define SPF_FEATURE_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

class feature {
public:
    bool state {true};
    std::string tag {};

    void set_state(bool val);

    virtual void on_reload();
    virtual void on_enable();
    virtual void on_disable();
    virtual void on_update();
    virtual void on_event(SDL_Event &sdl_event);
    virtual void on_render();
};

#endif