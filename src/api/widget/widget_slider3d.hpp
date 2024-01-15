#pragma once
#ifndef SPF_WIDGET_SLIDER3D_H
#define SPF_WIDGET_SLIDER3D_H

#include "api/widget/widget.hpp"
#include "api/ui/ui.hpp"

class widget_slider3d : public widget {
public:
    float text_offsets[2] {};
    std::string val_str {};

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