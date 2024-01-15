#include "feature.hpp"

void feature::set_state(bool val) {
    if (val == this->state) {
        return;
    }

    this->state = val;

    if (val) {
        this->on_enable();
    } else {
        this->on_disable();
    }
}

void feature::on_update() {

}

void feature::on_event(SDL_Event &sdl_event) {

}

void feature::on_render() {

}

void feature::on_enable() {

}

void feature::on_disable() {

}

void feature::on_reload() {

}
