#pragma once
#ifndef SPF_WIDGET_FRAME_H
#define SPF_WIDGET_FRAME_H

#include "widget.hpp"

class widget_frame : public widget {
public:
    bool dragging {false};
    bool resizing {false};

    math::rect resize_rect {};

    void on_reload() override;
    void on_enable() override;
    void on_disable() override;
    void on_pre_event(SDL_Event &sdl_event) override;
    void on_event(SDL_Event &sdl_event) override;
    void on_post_event(SDL_Event &sdl_event) override;
    void on_update() override;
    void on_render() override;
};

#endif