#include "widget_checkbox.hpp"
#include "utils/render.hpp"
#include "core/client.hpp"
#include "utils/input.hpp"

void widget_checkbox::on_disable() {
    widget::on_disable();
}

void widget_checkbox::on_event(SDL_Event &sdl_event) {
    widget::on_event(sdl_event);

    if (input::event["Mouse Motion"]) {
        client::change(this->highlight, this->hovered);
    } else if (this->hovered && input::event["Mouse Down"] && input::event["Mouse1"]) {
        client::change(this->focused, true);
    } else if (input::event["Mouse Up"]) {
        if (input::event["Mouse1 Up"] && this->hovered && this->focused) {
            auto val = (ui::boolean*) this->value;

            if (val != nullptr) {
                val->set_value(!val->get_value());

                ui::event.push_back(val);
                client::change(this->activy, val->get_value());
                client::core.widget_service.should_update = false;
            }
        }

        client::change(this->focused, false);
    }
}

void widget_checkbox::on_update() {
    widget::on_update();
    auto val = (ui::boolean*) this->value;

    if (this->activy != val->get_value()) {
        this->activy = val->get_value();

        ui::event.push_back(this->value);
        client::change(this->activy, val->get_value());
    }
}

void widget_checkbox::on_render() {
    widget::on_render();

    render::process(this->rect, client::theme().background_checkbox);
    render::process(this->extra + this->rect, client::theme().highlight_checkbox);

    if (this->highlight) {
        render::process(this->rect, client::theme().highlight_checkbox);
    }

    if (this->focused || this->activy) {
        render::process(this->extra + this->rect, client::theme().activy_checkbox);
    }

    render::process(this->tag, this->rect.x + this->offset.x, this->rect.y + this->offset.y, client::theme().string_checkbox);
}

void widget_checkbox::on_enable() {
    widget::on_enable();
}

void widget_checkbox::on_pre_event(SDL_Event &sdl_event) {
    widget::on_pre_event(sdl_event);
}

void widget_checkbox::on_post_event(SDL_Event &sdl_event) {
    widget::on_post_event(sdl_event);
}

void widget_checkbox::on_reload() {
    widget::on_reload();

    float text_width = render::get_text_width(this->tag);
    float text_height = render::get_text_height();
    float offset = text_height / 3;

    this->rect.w = std::max(this->rect.w, text_width + offset + text_height + offset + offset);
    this->rect.h = std::max(this->rect.h, text_height + offset);

    this->extra.w = text_height;
    this->extra.h = text_height;

    this->offset.x = offset;
    this->offset.w = offset * 2;
    this->offset.y = (this->rect.h / 2) - (text_height / 2);
    this->offset.h = offset;

    this->extra.x = this->rect.w - this->extra.w - offset;
    this->extra.y = (this->rect.h / 2) - (this->extra.h / 2);
}
