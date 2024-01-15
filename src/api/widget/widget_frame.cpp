#include "widget_frame.hpp"
#include "core/client.hpp"
#include "utils/render.hpp"
#include "utils/log.hpp"
#include "utils/input.hpp"

void widget_frame::on_reload() {
    widget::on_reload();

    float text_width = render::get_text_width(this->tag);
    float text_height = render::get_text_height();
    float offset = text_height / 3;

    this->rect.w = std::max(this->rect.w, text_width + offset * 2);
    this->offset.h = render::get_text_height() + offset;

    this->rect.h = std::max(this->rect.h, this->offset.h);
    this->offset.w = this->rect.w;

    this->offset.y = (this->offset.h / 2) - (text_height / 2);
    this->offset.x = offset;
}

void widget_frame::on_enable() {
    widget::on_enable();
}

void widget_frame::on_disable() {
    widget::on_disable();
}

void widget_frame::on_event(SDL_Event &sdl_event) {
    widget::on_event(sdl_event);

    if (input::event["Mouse Motion"] && (this->dragging || this->resizing)) {
        bool cancelled = false;

        if (cancelled) {
            this->dragging = false;
            this->resizing = false;
        }

        client::core.widget_service.should_redraw = true;

        if (this->dragging) {
            this->rect.x = input::mouse.x - this->extra.x;
            this->rect.y = input::mouse.y - this->extra.y;
            client::core.widget_service.dispatch_task_parent_scale(this);
        }
    } else if (this->hovered && input::event["Mouse Down"] && (input::event["Mouse3"] || input::event["Mouse1"])) {
        const math::rect resize_rect = math::rect(this->rect.x, this->rect.y, this->offset.w, this->offset.h);

        if (this->resize_rect.collide_with_ab(input::mouse.x, input::mouse.y)) {
        } else if (resize_rect.collide_with_ab(input::mouse.x, input::mouse.y)) {
            this->extra.x = input::mouse.x - this->rect.x;
            this->extra.y = input::mouse.y - this->rect.y;
            this->dragging = true;
        }
    } else if (input::event["Mouse Up"]) {
        this->dragging = false;
        this->resizing = false;
    }
}

void widget_frame::on_update() {
    widget::on_update();
}

void widget_frame::on_render() {
    widget::on_render();

    const math::rect resize_rect = math::rect(this->rect.x, this->rect.y, this->offset.w, this->offset.h);

    render::process(this->rect, client::theme().background_frame);
    render::process(resize_rect, client::theme().activy_frame);
    render::process(this->tag, this->rect.x + this->offset.x, this->rect.y + this->offset.y, client::theme().string_frame);
}

void widget_frame::on_pre_event(SDL_Event &sdl_event) {
    widget::on_pre_event(sdl_event);
}

void widget_frame::on_post_event(SDL_Event &sdl_event) {
    widget::on_post_event(sdl_event);
}
