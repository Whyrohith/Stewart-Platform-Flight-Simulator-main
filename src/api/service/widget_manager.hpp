#pragma once
#ifndef SPF_WIDGET_MANAGER_H
#define SPF_WIDGET_MANAGER_H

#include "api/widget/widget.hpp"
#include "api/ui/ui.hpp"
#include <SDL2/SDL.h>
#include <amogpu/amogpu.hpp>
#include "utils/render.hpp"

class dock_group {
public:
    int32_t size {};

    bool lock {};
    bool dispatched {};

    math::rect rect {};
    ui::dock dock {};

    bool dispatch_draw(std::vector<math::rect> &batch, const math::rect &rect_to_draw, const ui::dock &previous_dock, ui::dock &target);

    void start_process();
    void process_prepare_phase(widget* element);
    void process_metrics();
    void process_end_phase(widget* element);

    bool empty();
    void reset_counter();
    void count();
};

class widget_manager {
public:
    std::vector<widget*> widget_list {};
    std::vector<widget*> task_dock_widget_list {};
    std::vector<widget*> task_widget_list {};
    std::vector<widget*> task_update_list {};
    std::vector<math::rect> absolute_draw;

    std::map<uint32_t, bool> task_validation_fast {};
    std::map<uint32_t, bool> task_validation {};
    std::map<uint32_t, widget*> data {};

    font_renderer f_render_size_normal {};
    font_renderer f_render_size_high {};
    font_renderer f_render_size_small {};

    render::theme theme {};
    dynamic_batching batch {};
    shape_builder fast_tessellator;

    uint32_t focused_widget_id {};
    uint32_t forced_focused_widget_id {};
    uint32_t hovered_widget_id {};
    uint32_t previous_focused_widget_id {};

    uint32_t previous_event_up_widget_id {};
    uint32_t previous_event_down_widget_id {};

    bool should_redraw {};
    bool should_fix_stack {};
    bool should_reset_stack {};
    bool should_force_fix_stack {};
    bool should_update {};

    dock_group dock_group_left {};
    dock_group dock_group_right {};

    void dispatch_task_update(int32_t id);
    void fix_parent_scale(widget* element, math::rect &offset);
    void dispatch_task_parent_scale(widget* element);
    void fix_layout(uint32_t id);
    void fix_layout(widget* element, std::map<uint32_t, widget*> &map);
    void add(widget* element);

    bool contains_into_dock_list(uint32_t id);
    widget* get_widget_by_id(uint32_t id);

    void clear();
    void reload();

    void on_init();
    void on_quit();
    void on_event(SDL_Event &sdl_event);
    void on_update();
    void on_render();
};

#endif