#pragma once
#ifndef SPF_UI_H
#define SPF_UI_H

#include <SDL2/SDL.h>
#include "api/ui/ui.hpp"
#include "utils/math.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace ui {
    enum class place {
        free, axis, next, fill
    };

    enum class dock {
        free, left, right
    };

    struct identity_data {
        uint32_t id {};
        uint32_t parent_mother_id {};

        std::vector<uint32_t> id_list {};
    };

	enum class type {
		empty, group, button, checkbox, slider, slider3d, label
	};

	struct value {
        std::string extra_tag {};
		std::string tag {};

        ui::type type {};
        ui::identity_data data {};
        ui::place place {};

        void set_extra_tag(const std::string &new_extra_tag);
        std::string get_extra_tag();

        bool enabled {true};
    };

    extern std::map<uint32_t, value*> data;
    extern std::vector<value*> event;
    extern uint32_t token_id_registry;

    void update_ui_event();
    void event_dispatch(ui::value* val_class_event);
    bool event_listener();

    ui::value* event_poll();

#endif

#ifndef SPF_UI_EXTENSIONS_H
#define SPF_UI_EXTENSIONS_H

    struct group : public ui::value {
    public:
        math::rect rect {30, 30, 125, 30};
        ui::dock initial_dock {};

        void add(ui::value* val);
    };

    struct number : public ui::value {
    protected:
        float x {};
        float y {};

        float minimum {};
        float maximum {};

        bool rollback {};
        int32_t scale {4};
    public:
        void set_scale_factor(int32_t factor);
        int32_t get_scale_factor();

        void set_rollback_flag(bool state);
        bool get_rollback_flag();

        void set_minimum(float minimum_value);
        float get_minimum();

        void set_maximum(float maximum_value);
        float get_maximum();

        void set_x(float val);
        float get_x();

        void set_y(float val);
        float get_y();
    };

    struct boolean : public ui::value {
    protected:
        bool value_state {};
    public:
        void set_value(bool val);
        bool get_value();
    };

#endif

#ifndef SPF_UI_ACCESS_H
#define SPF_UI_ACCESS_H

    extern ui::group* concurrent_group;

    ui::group* category(const std::string &text, math::rect rect = math::rect(10.0f, 10.0f, 125.0f, 75.0f));
    ui::value* label(const std::string &text, ui::place place = ui::place::fill);
    ui::boolean* checkbox(const std::string &text, ui::place place = ui::place::axis);
    ui::boolean* button(const std::string &text, ui::place place = ui::place::axis);
    ui::number* slider(float min, float max, const std::string &text = "", ui::place place = ui::place::fill);
    ui::number* slider3d(const std::string &name, float range, ui::place place = ui::place::next);

    void push_back_group(ui::value* val);
    void end_group();
#endif
}