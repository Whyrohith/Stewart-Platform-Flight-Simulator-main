#pragma once
#ifndef SPF_WIDGET_SLIDER_H
#define SPF_WIDGET_SLIDER_H

#include "api/widget/widget.hpp"
#include "api/ui/ui.hpp"

class widget_slider : public widget {
public:
    bool pressed {};
    float minus_offset {};
    std::string showname;

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