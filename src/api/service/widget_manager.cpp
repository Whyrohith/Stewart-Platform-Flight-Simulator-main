#include "widget_manager.hpp"
#include "api/widget/widget_frame.hpp"
#include "api/widget/widget_button.hpp"
#include "api/widget/widget_checkbox.hpp"
#include "utils/log.hpp"
#include "api/widget/widget_slider.hpp"
#include "api/widget/widget_slider3d.hpp"
#include "utils/input.hpp"
#include "api/widget/widget_label.hpp"
#include "core/client.hpp"

bool dock_group::dispatch_draw(std::vector<math::rect> &batch, const math::rect &rect_to_draw, const ui::dock &previous_target, ui::dock &target) {
    math::rect mesh {};
    this->dispatched = false;
    target = ui::dock::free;

    switch (this->dock) {
        case ui::dock::free: {
            break;
        }

        case ui::dock::left: {
            if (rect_to_draw.x > input::screen::width / 2) {
                break;
            }

            this->dispatched = true;
            this->lock = this->rect.h > input::screen::height - client::theme().dock_offset.y;
            this->lock = this->lock || (this->rect.h + client::theme().dock_offset.y + rect_to_draw.h > input::screen::height);

            mesh.x = client::theme().dock_offset.x;
            mesh.w = rect_to_draw.w;

            if (this->lock) {
                this->size++;
                mesh.h += client::theme().dock_offset.y + rect_to_draw.h;
                mesh.h /= static_cast<float>(this->size);

                mesh.y = mesh.h + client::theme().dock_offset.y;
                mesh.y *= static_cast<float>(this->size);
                this->size--;

                mesh.h = rect_to_draw.h;
            } else {
                mesh.y = this->rect.y + this->rect.h + client::theme().dock_offset.y;
                mesh.h = rect_to_draw.h;
            }

            break;
        }

        case ui::dock::right: {
            if (rect_to_draw.x < input::screen::width / 2) {
                break;
            }

            this->dispatched = true;
            this->lock = this->rect.h > input::screen::height - client::theme().dock_offset.y;
            this->lock = this->lock || (this->rect.h + client::theme().dock_offset.y + rect_to_draw.h > input::screen::height);

            mesh.x = input::screen::width - rect_to_draw.w - client::theme().dock_offset.x;
            mesh.w = rect_to_draw.w;

            if (this->lock) {
                this->size++;
                mesh.h += client::theme().dock_offset.y + rect_to_draw.h;
                mesh.h /= static_cast<float>(this->size);

                mesh.y = mesh.h + client::theme().dock_offset.y;
                mesh.y *= static_cast<float>(this->size);
                this->size--;

                mesh.h = rect_to_draw.h;
            } else {
                mesh.y = this->rect.y + this->rect.h + client::theme().dock_offset.y;
                mesh.h = rect_to_draw.h;
            }

            break;
        }
    }

    bool flag {previous_target == this->dock};

    if (!flag) {
        target = ui::dock::free;
        batch.push_back(mesh);
    }

    if (rect_to_draw.collide_with_aabb(mesh)) {
        target = this->dock;
    }

    return this->dispatched;
}

bool dock_group::empty() {
    return this->size <= 1;
}

void dock_group::reset_counter() {
    this->size = 1;
}

void dock_group::count() {
    this->size++;
}

void dock_group::start_process() {
    this->reset_counter();

    switch (this->dock) {
        case ui::dock::free: {
            break;
        }

        case ui::dock::left: {
            this->rect.y = 0;
            this->rect.h = 0;

            break;
        }

        case ui::dock::right: {
            this->rect.y = 0;
            this->rect.h = 0;

            break;
        }
    }
}

void dock_group::process_prepare_phase(widget* element) {
    if (element->dock != this->dock) {
        return;
    }

    switch (element->dock) {
        case ui::dock::free: {
            break;
        }

        case ui::dock::left: {
            this->count();
            break;
        }

        case ui::dock::right: {
            this->count();
            break;
        }
    }
}

void dock_group::process_end_phase(widget* element) {
    if (element->dock != this->dock) {
        return;
    }

    switch (element->dock) {
        case ui::dock::free: {
            break;
        }

        case ui::dock::left: {
            element->rect.x = client::theme().dock_offset.x;
            element->rect.y = this->rect.y + this->rect.h + (this->empty() ? 0 : client::theme().dock_offset.y);

            if (this->lock) {
                element->rect.h = this->rect.h * static_cast<float>(this->size);
                this->count();
            } else {
                this->rect.y += client::theme().dock_offset.y + element->rect.h;
            }

            break;
        }

        case ui::dock::right: {
            element->rect.x = input::screen::width - element->rect.w - client::theme().dock_offset.x;
            element->rect.y = this->rect.y + this->rect.h + (this->empty() ? 0 : client::theme().dock_offset.y);

            if (this->lock) {
                element->rect.h = this->rect.h * static_cast<float>(this->size);
                this->count();
            } else {
                this->rect.y += client::theme().dock_offset.y + element->rect.h;
            }

            break;
        }
    }
}

void dock_group::process_metrics() {
    switch (this->dock) {
        case ui::dock::free: {
            break;
        }

        case ui::dock::left: {
            if (this->lock) {
                this->rect.h = input::screen::height - client::theme().dock_offset.y * 2;
                this->rect.h /= static_cast<float>(this->size);
                this->reset_counter();
            } else {

            }

            break;
        }

        case ui::dock::right: {
            if (this->lock) {
                this->rect.h = input::screen::height - client::theme().dock_offset.y * 2;
                this->rect.h /= static_cast<float>(this->size);
                this->reset_counter();
            } else {

            }

            break;
        }
    }
}

void widget_manager::dispatch_task_update(int32_t id) {
    if (this->data[id] == nullptr) {
        return;
    }

    this->task_update_list.push_back(this->data[id]);
    this->should_update = true;
}

void widget_manager::on_event(SDL_Event &sdl_event) {
    ui::update_ui_event();

    if (input::event["Window Size"]) {
        this->should_force_fix_stack = true;
    }

    this->focused_widget_id = 0;

    bool event_mouse_up = input::event["Mouse Up"];
    bool event_mouse_down = input::event["Mouse Down"];

    if (event_mouse_down || event_mouse_up) {
        this->previous_event_down_widget_id = 0;
        this->previous_event_up_widget_id = 0;
    }

    for (widget* &widgets : this->widget_list) {
        if (widgets == nullptr) {
            continue;
        }

        widgets->on_pre_event(sdl_event);

        if (widgets->hovered && widgets->enabled) {
            this->focused_widget_id = widgets->value->data.id;
        }

        widgets->on_post_event(sdl_event);
    }

    for (widget* &widgets : this->widget_list) {
        if (widgets == nullptr) {
            continue;
        }

        if (widgets->value->data.id == this->focused_widget_id && widgets->enabled) {
            widgets->on_pre_event(sdl_event);
        }

        widgets->on_event(sdl_event);
        widgets->on_post_event(sdl_event);
    }

    if (event_mouse_down && this->focused_widget_id != 0) {
        this->fix_layout(this->focused_widget_id);
        this->previous_event_down_widget_id = this->focused_widget_id;
    } else if (event_mouse_up && this->focused_widget_id != 0) {
        this->previous_event_up_widget_id = this->focused_widget_id;
    }
}

void widget_manager::on_update() {
    if (!this->task_widget_list.empty()) {
        math::rect initial_offset_rect {};

        for (widget* &widgets : this->task_widget_list) {
            if (widgets == nullptr) {
                continue;
            }

            initial_offset_rect.x = 0;
            initial_offset_rect.y = 0;
            initial_offset_rect.w = 0;
            initial_offset_rect.h = 0;

            this->fix_parent_scale(widgets, initial_offset_rect);
        }

        this->task_widget_list.clear();
        this->task_validation.clear();
        this->should_redraw = true;
    }

    if (((this->should_fix_stack && input::event["Mouse Up"]) || this->should_force_fix_stack)) {
        std::vector<widget*> fast_copy_list {};

        this->dock_group_left.start_process();
        this->dock_group_right.start_process();

        for (widget* &widgets : this->task_dock_widget_list) {
            if (widgets == nullptr) {
                continue;
            }

            widgets->previous_dock = widgets->dock;

            switch (widgets->dock) {
                case ui::dock::free: {
                    break;
                }

                default: {
                    fast_copy_list.push_back(widgets);

                    this->dock_group_left.process_prepare_phase(widgets);
                    this->dock_group_right.process_prepare_phase(widgets);

                    this->task_widget_list.push_back(widgets);

                    break;
                }
            }
        }

        this->task_dock_widget_list = fast_copy_list;

        this->dock_group_left.process_metrics();
        this->dock_group_right.process_metrics();

        for (widget* &widgets : this->task_dock_widget_list) {
            if (widgets == nullptr) {
                continue;
            }

            this->dock_group_left.process_end_phase(widgets);
            this->dock_group_right.process_end_phase(widgets);
        }

        this->should_fix_stack = false;
        this->should_force_fix_stack = false;
        this->should_redraw = true;
    }

    if (!this->task_update_list.empty()) {
        if (this->should_update) {
            for (widget* widgets : this->task_update_list) {
                if (widgets == nullptr) {
                    continue;
                }

                widgets->on_update();
            }
        }

        this->task_update_list.clear();
        this->should_update = false;
    }
}

void widget_manager::on_render() {
    if (this->should_redraw) {
        this->batch.invoke();

        for (widget* &widgets : this->widget_list) {
            if (widgets == nullptr) {
                continue;
            }

            if (widgets->enabled) {
                widgets->on_render();
            }
        }

        this->batch.revoke();
        this->should_redraw = false;
    }

    glDisable(GL_DEPTH_TEST);
    this->batch.draw();
    glEnable(GL_DEPTH_TEST);

    if (!this->absolute_draw.empty()) {
        this->fast_tessellator.invoke();

        amogpu::vec4f color = amogpu::vec4f(97.0f / 255, 141.0f / 255, 212.0f / 255, 0.2f);

        for (math::rect rects : this->absolute_draw) {
            fast_tessellator.build(amogpu::shape::RECT, color);
            fast_tessellator.draw(rects.x, rects.y, rects.w, rects.h);
        }

        this->fast_tessellator.revoke();

        if (this->previous_event_down_widget_id == 0) {
            this->absolute_draw.clear();
        }
    }
}

void widget_manager::clear() {
    this->task_dock_widget_list.clear();
    this->task_widget_list.clear();

    for (widget* &widgets : this->widget_list) {
        if (widgets == nullptr) {
            continue;
        }

        delete widgets;
    }

    this->widget_list.clear();
}

void widget_manager::reload() {
    uint32_t last_mother_frame_id = 0;
    this->data.clear();

    for (std::pair<int32_t, ui::value *> entry : ui::data) {
        auto value = entry.second;

        if (value == nullptr) {
            continue;
        }

        switch (value->type) {
            case ui::type::group: {
                if (value->data.parent_mother_id == 0) {
                    auto frame = new widget_frame();
                    auto group = (ui::group*) value;

                    frame->tag = value->tag;
                    frame->data = value->data;
                    frame->type = widget_type::frame;
                    frame->rect = group->rect;
                    frame->place = value->place;
                    frame->dock = group->initial_dock;
                    frame->value = value;
                    frame->on_reload();

                    this->add(frame);
                    this->dispatch_task_parent_scale(frame);
                    this->task_dock_widget_list.push_back(frame);
                    this->data[value->data.id] = frame;

                    last_mother_frame_id = frame->value->data.id;
                } else {

                }

                break;
            }

            case ui::type::button: {
                auto button = new widget_button();

                button->tag = value->tag;
                button->tag = value->tag;
                button->data = value->data;
                button->type = widget_type::button;
                button->place = value->place;
                button->value = value;
                button->on_reload();

                this->add(button);
                this->data[value->data.id] = button;

                break;
            }

            case ui::type::checkbox: {
                auto button = new widget_checkbox();

                button->tag = value->tag;
                button->tag = value->tag;
                button->data = value->data;
                button->type = widget_type::checkbox;
                button->place = value->place;
                button->value = value;
                button->on_reload();

                this->add(button);
                this->data[value->data.id] = button;

                break;
            }

            case ui::type::slider: {
                auto slider = new widget_slider();

                slider->tag = value->tag;
                slider->tag = value->tag;
                slider->data = value->data;
                slider->type = widget_type::slider;
                slider->place = value->place;
                slider->value = value;
                slider->on_reload();

                this->add(slider);
                this->data[value->data.id] = slider;

                break;
            }

            case ui::type::slider3d: {
                auto slider3d = new widget_slider3d();

                slider3d->tag = value->tag;
                slider3d->tag = value->tag;
                slider3d->data = value->data;
                slider3d->type = widget_type::slider3d;
                slider3d->place = value->place;
                slider3d->value = value;
                slider3d->on_reload();

                this->add(slider3d);
                this->data[value->data.id] = slider3d;

                break;
            }

            case ui::type::label: {
                auto label = new widget_label();

                label->tag = value->tag;
                label->tag = value->tag;
                label->data = value->data;
                label->type = widget_type::label;
                label->place = value->place;
                label->value = value;
                label->on_reload();

                this->add(label);
                this->data[value->data.id] = label;

                break;
            }
        }
    }

    this->fix_layout(last_mother_frame_id);

    this->dock_group_left.dock = ui::dock::left;
    this->dock_group_right.dock = ui::dock::right;

    this->should_update = false;
    this->should_force_fix_stack = true;
}

void widget_manager::on_init() {
    this->f_render_size_small.load(relative_path + "data/JetBrainsMono-Light.ttf", 14);
    this->f_render_size_normal.load(relative_path + "data/JetBrainsMono-Light.ttf", 18);
    this->f_render_size_high.load(relative_path + "data/JetBrainsMono-Light.ttf", 24);

    this->clear();
    this->reload();
}

void widget_manager::on_quit() {
    for (widget* &widgets : this->widget_list) {
        if (widgets == nullptr) {
            continue;
        }

        delete widgets;
    }
}

void widget_manager::add(widget *element) {
    element->enabled = true;

    this->widget_list.push_back(element);
    this->should_redraw = true;
    this->should_fix_stack = true;
    this->forced_focused_widget_id = element->value->data.id;
}

void widget_manager::fix_parent_scale(widget *element, math::rect &offset) {
    if (element == nullptr) {
        return;
    }

    float y_user_offset {};
    float x_user_offset {};
    float largest_width {};

    y_user_offset = client::theme().parent_offset.y;
    x_user_offset = client::theme().parent_offset.x;

    switch (element->type) {
        case widget_type::category: {
            auto group = (widget_frame*) element;

            if (largest_width > offset.w) {
                offset.w = largest_width;
                offset.h += y_user_offset;
            }

            break;
        }

        case widget_type::frame: {
            auto group = (widget_frame*) element;

            largest_width = element->rect.w;
            this->absolute_draw.clear();

            offset.w = element->rect.w;
            offset.h += element->offset.h + y_user_offset;

            bool push_back_to_dock_list = group->dragging;
            bool dock_changed {};

            ui::dock dock_target {};
            this->dock_group_right.dispatch_draw(this->absolute_draw, element->rect, element->previous_dock, dock_target) || this->dock_group_left.dispatch_draw(this->absolute_draw, element->rect, element->previous_dock, dock_target);

            dock_changed = element->dock != dock_target;

            if (group->dragging && dock_changed) {
                this->should_fix_stack = true;
                element->dock = dock_target;
            }

            if (!this->contains_into_dock_list(element->value->data.id) && dock_changed && group->dragging) {
                this->task_dock_widget_list.push_back(element);
            }

            break;
        }
    }

    offset.x = x_user_offset;

    bool prev_next {};
    bool prev_fill {};
    bool prev_axis {};

    float prev_height_axis {};
    float prev_height_next {};
    int32_t it_count {};

    std::vector<widget*> concurrent_fast_widget_list {};

    for (uint32_t &ids : element->value->data.id_list) {
        it_count++;

        if (ids == element->value->data.id) {
            continue;
        }

        auto widgets = this->get_widget_by_id(ids);

        if (widgets == nullptr || !widgets->enabled) {
            continue;
        }

        widgets->rect.y = element->rect.y + offset.h;

        switch (widgets->type) {
            case widget_type::category: {
                this->fix_parent_scale(widgets, offset);
                break;
            }

            case widget_type::slider3d: {
                widgets->place = ui::place::fill;
                ((widget_slider3d*) widgets)->place = ui::place::fill;
            };
        }

        switch (widgets->place) {
            case ui::place::axis: {
                widgets->rect.x = element->rect.x + offset.x;
                offset.x += widgets->rect.w + x_user_offset;

                if (widgets->rect.h > prev_height_axis) {
                    prev_height_axis = widgets->rect.h;
                }

                largest_width = offset.x;
                prev_height_next = 0;

                break;
            }

            case ui::place::next: {
                if (prev_axis && prev_height_axis > 0) {
                    offset.h += prev_height_axis + y_user_offset;
                    widgets->rect.y = element->rect.y + offset.h;
                    prev_height_axis = 0;
                } else if (!prev_fill) {
                    offset.h += widgets->rect.h + y_user_offset;
                    widgets->rect.y = element->rect.y + offset.h;
                }

                offset.x = x_user_offset;
                widgets->rect.x = element->rect.x + offset.x;
                largest_width = widgets->rect.w + (x_user_offset * 2);
                offset.x += widgets->rect.w + x_user_offset;

                prev_next = true;
                prev_height_next = widgets->rect.h;

                break;
            }

            case ui::place::fill: {
                if (prev_next && prev_height_next > 0) {
                    offset.h += prev_height_next + y_user_offset;
                    widgets->rect.y = element->rect.y + offset.h;
                    prev_height_next = 0;
                } else if (prev_axis && prev_height_axis > 0) {
                    offset.h += prev_height_axis + y_user_offset;
                    widgets->rect.y = element->rect.y + offset.h;
                    prev_height_axis = 0;
                }

                offset.h += widgets->rect.h + y_user_offset;
                offset.x = x_user_offset;

                largest_width = widgets->rect.w + (x_user_offset * 2);
                widgets->rect.x = element->rect.x + offset.x;

                break;
            }
        }

        prev_fill = widgets->place == ui::place::fill;
        prev_next = widgets->place == ui::place::next;
        prev_axis = widgets->place == ui::place::axis;

        if (largest_width > offset.w) {
            offset.w = largest_width;
        }

        if (it_count == element->value->data.id_list.size()) {
            offset.h += prev_height_axis + y_user_offset;
        }

        concurrent_fast_widget_list.push_back(widgets);
    }

    if (offset.h > element->rect.h || element->dock == ui::dock::free) {
        element->rect.h = offset.h;
    }

    if (offset.w > element->rect.w && element->type == widget_type::frame) {
        element->rect.w = offset.w;
    }

    element->on_reload();
    it_count = 0;

    for (widget* &widgets : concurrent_fast_widget_list) {
        if (widgets == nullptr) {
            continue;
        }

        switch (widgets->place) {
            case ui::place::fill: {
                widgets->rect.w = element->rect.w - (x_user_offset * 2);
                widgets->on_reload();
                break;
            }

            default: {
                switch (widgets->type) {
                    case widget_type::slider: {
                        if (widgets->tag.empty()) {
                            bool keep {};
                            float prev {};

                            if (it_count + 1 == concurrent_fast_widget_list.size() || it_count == concurrent_fast_widget_list.size()) {
                                keep = true;
                            } else {
                                keep = concurrent_fast_widget_list[it_count + 1] != nullptr && concurrent_fast_widget_list[it_count + 1]->place != ui::place::axis;
                            }

                            if (keep && (it_count + 1 <= concurrent_fast_widget_list.size() || (it_count + 1 < concurrent_fast_widget_list.size() && it_count - 1 > 0 && (concurrent_fast_widget_list[it_count + 1] == nullptr || concurrent_fast_widget_list[it_count + 1]->place != ui::place::axis)))) {
                                prev = (element->rect.x + element->rect.w) - (concurrent_fast_widget_list[it_count - 1]->rect.x + concurrent_fast_widget_list[it_count - 1]->rect.w);
                            } else {
                                keep = false;
                            }

                            if (keep) {
                                widgets->rect.w = (prev) - x_user_offset * 2;
                            }
                        }
                    }
                }

                break;
            }
        }

        widgets->mother = element->rect;
        it_count++;
    }
}

widget *widget_manager::get_widget_by_id(uint32_t id) {
    for (widget* &widgets : this->widget_list) {
        if (widgets == nullptr) {
            continue;
        }

        if (widgets->value->data.id == id) {
            return widgets;
        }
    }

    return nullptr;
}

void widget_manager::dispatch_task_parent_scale(widget *element) {
    if (element == nullptr || this->task_validation[element->value->data.id]) {
        return;
    }

    this->task_widget_list.push_back(element);
    this->task_validation[element->value->data.id] = true;
}

void widget_manager::fix_layout(uint32_t id) {
    if (id == 0) {
        return;
    }

    if (this->previous_focused_widget_id != id) {
        this->previous_focused_widget_id = id;
        this->should_redraw = true;
    }

    std::map<uint32_t, widget*> fast_map {};
    std::map<uint32_t, widget*> fast_stack_map {};
    std::map<uint32_t, widget*> fast_focused_stack_map {};

    std::vector<widget*> concurrent_fast_widget_list {};

    for (widget* &widgets : this->widget_list) {
        if (widgets == nullptr) {
            continue;
        }

        if (fast_stack_map[widgets->value->data.id] != nullptr || fast_focused_stack_map[widgets->value->data.id] != nullptr) {
            continue;
        }

        fast_map.clear();
        this->fix_layout(widgets, fast_map);

        if (fast_map[id] != nullptr) {
            fast_focused_stack_map = fast_map;
        } else {
            for (std::pair<uint32_t, widget*> entry : fast_map) {
                fast_stack_map[entry.first] = entry.second;
            }
        }
    }

    for (std::pair<uint32_t, widget*> entry : fast_stack_map) {
        if (entry.second == nullptr) {
            continue;
        }

        concurrent_fast_widget_list.push_back(entry.second);
    }

    for (std::pair<uint32_t, widget*> entry : fast_focused_stack_map) {
        if (entry.second == nullptr) {
            continue;
        }

        concurrent_fast_widget_list.push_back(entry.second);
    }

    this->widget_list.clear();
    this->widget_list = concurrent_fast_widget_list;
}

void widget_manager::fix_layout(widget *element, std::map<uint32_t, widget*> &map) {
    if (element == nullptr || map[element->value->data.id] != nullptr) {
        return;
    }

    map[element->value->data.id] = element;

    for (uint32_t &id : element->value->data.id_list) {
        auto widget = this->get_widget_by_id(id);

        if (widget == nullptr) {
            continue;
        }

        map[widget->value->data.id] = widget;

        if (widget->type == widget_type::category || widget->type == widget_type::frame) {
            this->fix_layout(widget, map);
        }
    }
}

bool widget_manager::contains_into_dock_list(uint32_t id) {
    for (widget* &widgets : this->task_dock_widget_list) {
        if (widgets->value->data.id == id) {
            return true;
        }
    }

    return false;
}
