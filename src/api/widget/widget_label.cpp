#include "widget_label.hpp"
#include "core/client.hpp"

void widget_label::on_reload() {
    widget::on_reload();

    float text_width = render::get_text_width(this->tag);
    float text_height = render::get_text_height();
    float offset = text_height / 3;

    this->rect.w = std::max(this->rect.w, text_width + offset * 2);
    this->rect.h = std::max(this->rect.h, text_height + offset);

    this->offset.x = offset;
    this->offset.y = (this->rect.h / 2) - (text_height / 2);
}

void widget_label::on_enable() {
    widget::on_enable();
}

void widget_label::on_disable() {
    widget::on_disable();
}

void widget_label::on_pre_event(SDL_Event &sdl_event) {
    widget::on_pre_event(sdl_event);
}

void widget_label::on_event(SDL_Event &sdl_event) {
    widget::on_event(sdl_event);
}

void widget_label::on_post_event(SDL_Event &sdl_event) {
    widget::on_post_event(sdl_event);
}

void widget_label::on_update() {
    widget::on_update();
    auto val = (ui::number*) this->value;

    if (this->tag != val->tag) {
        this->tag = val->tag;
        client::core.widget_service.should_redraw = true;
    }
}

void widget_label::on_render() {
    widget::on_render();
    render::process(this->tag, this->rect.x + this->offset.x, this->rect.y + this->offset.y, client::theme().label_string);
}
