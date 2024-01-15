#include "widget_button.hpp"
#include "utils/input.hpp"
#include "core/client.hpp"
#include "api/ui/ui.hpp"

void widget_button::on_enable() {
    widget::on_enable();
}

void widget_button::on_disable() {
    widget::on_disable();
}

void widget_button::on_event(SDL_Event &sdl_event) {
    widget::on_event(sdl_event);

    if (input::event["Mouse Motion"]) {
        client::change(this->highlight, this->hovered);
    } else if (this->hovered && input::event["Mouse Down"] && input::event["Mouse1"]) {
        client::change(this->focused, true);
    } else if (input::event["Mouse Up"]) {
        if (input::event["Mouse1 Up"] && this->hovered) {
            auto val = (ui::boolean*) this->value;

            if (val != nullptr) {
                val->set_value(true);
                ui::event.push_back(val);
                client::core.widget_service.should_update = false;
            }
        }

        client::change(this->focused, false);
    }
}

void widget_button::on_update() {
    widget::on_update();
}

void widget_button::on_render() {
    widget::on_render();
    render::process(this->rect, client::theme().background_button);

    if (this->highlight) {
        render::process(this->rect, client::theme().highlight_button);
    }

    if (this->focused) {
        render::process(this->rect, client::theme().activy_button);
    }

    render::process(this->tag, this->rect.x + this->offset.x, this->rect.y + this->offset.y, client::theme().string_button);
}

void widget_button::on_reload() {
    widget::on_reload();

    float text_width = render::get_text_width(this->tag);
    float text_height = render::get_text_height();
    float offset = text_height / 3;

    this->rect.w = std::max(this->rect.w, text_width + offset * 2);
    this->rect.h = std::max(this->rect.h, text_height + offset);

    switch (this->place) {
        case ui::place::fill: {
            this->offset.x = offset;
            break;
        }

        default: {
            this->offset.x = (this->rect.w / 2) - (text_width / 2);
            break;
        }
    }

    this->offset.w = offset * 2;
    this->offset.y = (this->rect.h / 2) - (text_height / 2);
    this->offset.h = offset;
}

void widget_button::on_pre_event(SDL_Event &sdl_event) {
    widget::on_pre_event(sdl_event);
}

void widget_button::on_post_event(SDL_Event &sdl_event) {
    widget::on_post_event(sdl_event);
}
