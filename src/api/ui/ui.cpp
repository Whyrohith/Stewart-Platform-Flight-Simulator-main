#include "ui.hpp"
#include "core/client.hpp"

ui::group* ui::concurrent_group {};
uint32_t ui::token_id_registry {1};

std::vector<ui::value*> ui::event {};
std::map<uint32_t, ui::value*> ui::data {};

void ui::boolean::set_value(bool val) {
    if (this->value_state != val) {
        this->value_state = val;
        client::core.widget_service.dispatch_task_update(this->data.id);
    }
}

bool ui::boolean::get_value() {
    return this->value_state;
}

void ui::number::set_scale_factor(int32_t factor) {
    if (this->scale != factor) {
        this->scale = factor;
        client::core.widget_service.dispatch_task_update(this->data.id);
    }
}

int32_t ui::number::get_scale_factor() {
    return this->scale;
}

void ui::number::set_rollback_flag(bool state) {
    if (this->rollback != state) {
        this->rollback = state;
        client::core.widget_service.dispatch_task_update(this->data.id);
    }
}

bool ui::number::get_rollback_flag() {
    return this->rollback;
}

void ui::number::set_minimum(float minimum_value) {
    if (this->minimum != minimum_value) {
        this->minimum = minimum_value;
        client::core.widget_service.dispatch_task_update(this->data.id);
    }
}

float ui::number::get_minimum() {
    return this->minimum;
}

void ui::number::set_maximum(float maximum_value) {
    if (this->maximum != maximum_value) {
        this->maximum = maximum_value;
        client::core.widget_service.dispatch_task_update(this->data.id);
    }
}

float ui::number::get_maximum() {
    return this->maximum;
}

void ui::number::set_x(float val) {
    val = std::min(this->maximum, std::max(this->minimum, val));

    if (this->x != val) {
        this->x = val;
        client::core.widget_service.dispatch_task_update(this->data.id);
    }
}

float ui::number::get_x() {
    return this->x;
}

void ui::number::set_y(float val) {
    val = std::min(this->maximum, std::max(this->minimum, val));

    if (this->y != val) {
        this->y = val;
        client::core.widget_service.dispatch_task_update(this->data.id);
    }
}

float ui::number::get_y() {
    return this->y;
}

ui::boolean *ui::checkbox(const std::string &text, ui::place place) {
    auto val = new ui::boolean();
    val->data.id = ui::token_id_registry++;

    ui::data[val->data.id] = val;
    ui::push_back_group(val);

    val->tag = text;
    val->type = ui::type::checkbox;
    val->set_value(true);
    val->place = place;

    return val;
}

ui::boolean *ui::button(const std::string &text, ui::place place) {
    auto val = new ui::boolean();
    val->data.id = ui::token_id_registry++;

    ui::data[val->data.id] = val;
    ui::push_back_group(val);

    val->tag = text;
    val->type = ui::type::button;
    val->place = place;
    val->set_value(false);

    return val;
}

ui::number *
ui::slider(float min, float max, const std::string &text, ui::place place) {
    auto val = new ui::number();
    val->data.id = ui::token_id_registry++;

    ui::data[val->data.id] = val;
    ui::push_back_group(val);

    val->tag = text;
    val->type = ui::type::slider;
    val->place = place;

    val->set_x(min);
    val->set_maximum(max);
    val->set_minimum(min);

    return val;
}

ui::number *
ui::slider3d(const std::string &text, float range, ui::place place) {
    auto val = new ui::number();
    val->data.id = ui::token_id_registry++;

    ui::data[val->data.id] = val;
    ui::push_back_group(val);

    val->tag = text;
    val->type = ui::type::slider3d;
    val->place = place;

    val->set_x(0);
    val->set_y(0);
    val->set_maximum(range);
    val->set_minimum(-range);

    return val;
}

ui::group *ui::category(const std::string &text, math::rect rect) {
    auto val = new ui::group();
    val->data.id = ui::token_id_registry++;

    ui::data[val->data.id] = val;
    ui::push_back_group(val);

    val->tag = text;
    val->type = ui::type::group;
    val->place = ui::place::free;
    val->rect = rect;

    concurrent_group = val;
    return val;
}

void ui::push_back_group(ui::value *val) {
    if (ui::concurrent_group != nullptr) {
        ui::concurrent_group->add(val);
    }
}

ui::value *
ui::label(const std::string &text, ui::place place) {
    auto val = new ui::value();
    val->data.id = ui::token_id_registry++;

    ui::data[val->data.id] = val;
    ui::push_back_group(val);

    val->tag = text;
    val->type = ui::type::label;
    val->place = place;

    return val;
}

void ui::end_group() {
    ui::concurrent_group = nullptr;
}

void ui::update_ui_event() {
    if (!ui::event.empty()) {
        ui::event.clear();
    }
}

ui::value *ui::event_poll() {
    if (ui::event.empty()) {
        return nullptr;
    }

    return ui::event.at(0);
}

bool ui::event_listener() {
    return !ui::event.empty();
}

void ui::event_dispatch(ui::value* val_class_event) {
    ui::event.push_back(val_class_event);
}

void ui::group::add(ui::value *val) {
    if (val == nullptr) {
        return;
    }

    val->data.parent_mother_id = this->data.id;
    this->data.id_list.push_back(val->data.id);
}
