#include "widget_slider.hpp"
#include "utils/render.hpp"
#include "core/client.hpp"
#include "utils/input.hpp"

void widget_slider::on_enable() {
    widget::on_enable();
}

void widget_slider::on_disable() {
    widget::on_disable();
}

void widget_slider::on_event(SDL_Event &sdl_event) {
    widget::on_event(sdl_event);

    if (input::event["Mouse Up"] || (this->focused && input::event["Mouse Down"])) {
        client::change(this->focused, false);
    }

    if (input::event["Mouse1"] && this->hovered && input::event["Mouse Down"]) {
        client::change(this->focused, true);
    }

    if (input::event["Mouse Motion"] || input::event["Mouse1"]) {
        client::change(this->highlight, this->hovered);

        if (this->focused) {
            auto val = (ui::number*) this->value;

            float max = val->get_maximum();
            float min = val->get_minimum();

            float size = this->rect.w - (this->minus_offset * 2);
            float diff = std::min(size, std::max(0.0f, input::mouse.x - (this->extra + this->rect).x));
            float val_float {};

            if (diff == 0) {
                val_float = min;
            } else {
                val_float = (diff / size) * (max - min) + min;
            }

            if (val->get_x() != val_float) {
                val->set_x(val_float);

                this->on_reload();
                ui::event.push_back(val);
                
                client::core.widget_service.should_redraw = true;
                client::core.widget_service.should_update = false;
            }
        }
    }
}

void widget_slider::on_update() {
    widget::on_update();
    this->on_reload();
}

void widget_slider::on_render() {
    widget::on_render();
    render::process(this->rect, client::theme().background_slider);

    auto val = (ui::number*) this->value;
    this->extra.w = (this->rect.w - this->minus_offset * 2) * (val->get_x() - val->get_minimum()) / (val->get_maximum() - val->get_minimum());

    if (this->highlight || this->focused) {
        render::process(this->rect, client::theme().highlight_slider);
    }

    render::process(this->extra + this->rect, client::theme().activy_slider);
    render::process(this->showname, this->rect.x + this->offset.x, this->rect.y + this->offset.y, client::theme().string_slider);
}

void widget_slider::on_pre_event(SDL_Event &sdl_event) {
    widget::on_pre_event(sdl_event);
}

void widget_slider::on_post_event(SDL_Event &sdl_event) {
    widget::on_post_event(sdl_event);
}

void widget_slider::on_reload() {
    widget::on_reload();

    std::string x = this->tag.empty() ? "" : this->tag + ": ";
    auto val = (ui::number*) this->value;

    this->showname = std::to_string(val->get_maximum());
    this->showname = x + this->showname.substr(0, this->showname.find('.') + 3);

    float text_width = render::get_text_width(this->showname);
    float text_height = render::get_text_height();

    this->showname = std::to_string(val->get_x());
    this->showname = x + this->showname.substr(0, this->showname.find('.') + 3);
    this->minus_offset = text_height / 3;

    this->rect.w = std::max(this->rect.w, text_width + this->minus_offset * 2);
    this->rect.h = std::max(this->rect.h, text_height + this->minus_offset);

    switch (this->place) {
        case ui::place::fill: {
            this->offset.x = this->minus_offset;
            break;
        }

        default: {
            this->offset.x = (this->rect.w / 2) - (text_width / 2);
            break;
        }
    }

    this->offset.w = this->minus_offset * 2;
    this->offset.y = (this->rect.h / 2) - (text_height / 2);
    this->offset.h = this->minus_offset;

    this->minus_offset -= this->minus_offset / 1.5f;
    this->extra.h = text_height;
    this->extra.x = this->minus_offset;
    this->extra.y = this->offset.y;
}
