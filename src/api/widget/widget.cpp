#include "widget.hpp"
#include "utils/input.hpp"

void widget::on_enable() {
    feature::on_enable();
}

void widget::on_disable() {
    feature::on_disable();
}

void widget::on_event(SDL_Event &sdl_event) {
    feature::on_event(sdl_event);
}

void widget::on_update() {
    feature::on_update();
}

void widget::on_render() {
    feature::on_render();
}

void widget::on_reload() {

}

widget::widget() {

}

widget::~widget() {

}

void widget::on_pre_event(SDL_Event &sdl_event) {
    if (input::event["Mouse Down"] || input::event["Mouse Up"] || input::event["Mouse Motion"]) {
        this->hovered = this->rect.collide_with_ab(input::mouse.x, input::mouse.y) && (this->data.parent_mother_id == 0 || this->mother.collide_with_ab(input::mouse.x, input::mouse.y));
    }
}

void widget::on_post_event(SDL_Event &sdl_event) {
    this->hovered = false;
}
