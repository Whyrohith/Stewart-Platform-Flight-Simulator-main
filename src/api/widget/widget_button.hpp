#pragma once
#ifndef SPF_WIDGET_BUTTON_H
#define SPF_WIDGET_BUTTON_H

#include "api/widget/widget.hpp"

class widget_button : public widget {
public:
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