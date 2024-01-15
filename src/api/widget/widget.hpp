#pragma once
#ifndef SPF_WIDGET_H
#define SPF_WIDGET_H

#include "api/feature/feature.hpp"
#include "utils/math.hpp"
#include "utils/log.hpp"
#include "api/ui/ui.hpp"
#include "utils/log.hpp"
#include <vector>

enum class widget_type {
    abstract, frame, button, category, checkbox, label, slider, slider3d
};

class widget : public feature {
public:
    widget();
    ~widget();

    ui::identity_data data {};
    ui::place place {};
    ui::dock dock {};
    ui::dock previous_dock {};
    ui::value* value {};

    widget_type type {};

    math::rect rect;
    math::rect offset;
    math::rect extra;
    math::rect mother;

    bool enabled {true};
    bool hovered {};
    bool highlight {};
    bool focused {};
    bool activy {};

    virtual void on_reload();
    virtual void on_pre_event(SDL_Event &sdl_event);
    virtual void on_post_event(SDL_Event &sdl_event);

    void on_enable() override;
    void on_disable() override;
    void on_event(SDL_Event &sdl_event) override;
    void on_update() override;
    void on_render() override;
};

#endif