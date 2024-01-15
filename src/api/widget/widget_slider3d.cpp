#include "widget_slider3d.hpp"
#include "utils/render.hpp"
#include "core/client.hpp"
#include "utils/input.hpp"

void widget_slider3d::on_enable() {
    widget::on_enable();
}

void widget_slider3d::on_disable() {
    widget::on_disable();
}

void widget_slider3d::on_event(SDL_Event &sdl_event) {
    widget::on_event(sdl_event);
    
    bool drag {};
    auto val = (ui::number*) this->value;

    if (input::event["Mouse Up"] || (this->focused && input::event["Mouse Down"])) {
        if (val->get_rollback_flag() && input::event["Mouse Up"] && this->focused) {
            this->extra.w = this->rect.w / 2;
            this->extra.h = this->rect.h / 2;

            drag = true;
        }

        client::change(this->focused, false);
    }

    if (input::event["Mouse1"] && this->hovered && input::event["Mouse Down"]) {
        client::change(this->focused, true);
    }

    if (this->focused && (input::event["Mouse Motion"] || input::event["Mouse Down"])) {
        this->extra.w = std::min(this->rect.w, std::max(0.0f, input::mouse.x - this->rect.x));
        this->extra.h = std::min(this->rect.h, std::max(0.0f, input::mouse.y - this->rect.y));

        drag = true;
    }

    if (drag) {
        float v = val->get_maximum() * 2;

        this->extra.x = ((this->extra.w / this->rect.w) * v) - val->get_maximum();
        this->extra.y = val->get_maximum() - ((this->extra.h / this->rect.h) * v);

        this->extra.x = std::min(val->get_maximum(), std::max(val->get_minimum(), this->extra.x));
        this->extra.y = std::min(val->get_maximum(), std::max(val->get_minimum(), this->extra.y));

        val->set_x(this->extra.x);
        val->set_y(this->extra.y);

        this->on_reload();
        ui::event_dispatch(val);

        client::core.widget_service.should_update = false;
        client::core.widget_service.should_redraw = true;
    }
}

void widget_slider3d::on_update() {
    widget::on_update();
    auto val = (ui::number*) this->value;

    if (this->extra.x != val->get_x() || this->extra.y != val->get_y()) {
        this->extra.x = val->get_x();
        this->extra.y = val->get_y();

        this->extra.w = this->extra.x * this->rect.w;
        this->extra.h = this->extra.y * this->rect.h;

        this->on_reload();

        ui::event_dispatch(this->value);
        client::core.widget_service.should_redraw = true;
    }
}

void widget_slider3d::on_render() {
    widget::on_render();
    render::process(this->rect, client::theme().background_slider3d);

    render::process(this->rect.x, this->rect.y + this->extra.h, this->rect.w, 1, client::theme().activy_slider3d);
    render::process(this->rect.x + this->extra.w, this->rect.y, 1, this->rect.h, client::theme().activy_slider3d);

    render::process(this->tag, this->rect.x + this->offset.x, this->rect.y + this->text_offsets[0], client::theme().string_slider3d, render::font::small);
    render::process(val_str, this->rect.x + this->offset.x, this->rect.y + this->text_offsets[1], client::theme().string_slider3d, render::font::small);
}

void widget_slider3d::on_pre_event(SDL_Event &sdl_event) {
    widget::on_pre_event(sdl_event);
}

void widget_slider3d::on_post_event(SDL_Event &sdl_event) {
    widget::on_post_event(sdl_event);
}

void widget_slider3d::on_reload() {
    widget::on_reload();

    float text_height_small = render::get_text_height(render::font::small);
    float text_height = render::get_text_height();
    float offset = text_height_small / 3;

    auto val {(ui::number*) this->value};
    float max {val->get_maximum()};

    this->extra.w = ((this->extra.x + max) / (max * 2)) * this->rect.w;
    this->extra.h = (this->rect.h) - ((this->extra.y + max) / (max * 2)) * this->rect.h;

    this->extra.w = std::min(this->rect.w, std::max(0.0f, this->extra.w));
    this->extra.h = std::min(this->rect.h, std::max(0.0f, this->extra.h));

    std::string x = std::to_string(this->extra.x);
    x = x.substr(0, x.find('.') + 3);

    std::string y = std::to_string(this->extra.y);
    y = y.substr(0, y.find('.') + 3);

    val_str = "(" + x + ", " + y + ")";
    this->text_offsets[0] = offset;
    this->text_offsets[1] = this->text_offsets[0] + text_height_small + offset;

    this->offset.x = offset;
    this->rect.h = text_height * static_cast<float>(val->get_scale_factor());
}
