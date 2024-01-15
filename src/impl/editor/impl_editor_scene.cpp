#include <glm/ext/matrix_transform.hpp>
#include "impl_editor_scene.hpp"
#include "utils/input.hpp"
#include "core/client.hpp"
#include "utils/log.hpp"
#include "utils/math.hpp"

void impl_editor_scene::on_enable() {
    feature::on_enable();

    client::core.camera.position.y = 1.0;
    client::core.module_service.registry(new module_task("reset-pistons", this, [](feature* main) {
        auto editor = (impl_editor_scene*) main;

        for (int32_t i = 0; i < 6; i++) {
            const std::string str = "piston" + std::to_string(i);
            auto piston = (impl_piston_object*) editor->get_object_by_tag(str);

            if (piston == nullptr) {
                continue;
            }

            auto vec_inferior = editor->inferior->get_vertex(i);
            auto vec_superior = editor->superior->get_vertex(i);

            piston->set_size(vec_inferior, vec_superior);
            piston->on_matrix();
        }

        editor->superior->set_pos(glm::vec3(0, editor->the_piston0->base_force_height + (editor->the_piston0->base_force_height / 2), 0));

        return 0;
    }, true));

    client::core.module_service.registry(new module_task("refresh-pistons-angle", this, [](feature* main) {
        auto editor = (impl_editor_scene*) main;
        auto s = editor->superior;

        float s_high_y {};
        float s_low_y {666.0f};
        float base {editor->the_piston0->base_force_height};

        s->set_pos(glm::vec3(0, base + (base / 2), 0));
        glm::vec3 s_prev_vertex {};

        for (int32_t i = 0; i < 6; i++) {
            const std::string str = "piston" + std::to_string(i);
            auto piston = (impl_piston_object*) editor->get_object_by_tag(str);

            if (piston == nullptr) {
                continue;
            }

            auto vec_inferior = editor->inferior->get_vertex(i);
            auto vec_superior = s->get_vertex(i);

            if (vec_superior.y > s_high_y) {
                s_high_y = vec_superior.y;
            }

            if (vec_superior.y < s_low_y) {
                s_low_y = vec_superior.y;
            }

            piston->force->set_x(((vec_superior.y - base) / base));
            piston->set_angle(vec_inferior, vec_superior);
            s_prev_vertex = vec_superior;
        }

        auto &s_matrix = s->model_plane.get_default_matrix();

        float yaw_limit = editor->setting_max_yaw->get_x();
        float roll_limit = editor->setting_max_roll->get_x();

        if (s_matrix.rotation3f.x > yaw_limit) {
            s_matrix.rotation3f.x = yaw_limit;
        } else if (s_matrix.rotation3f.x < -yaw_limit) {
            s_matrix.rotation3f.x = -yaw_limit;
        }

        if (s_matrix.rotation3f.z > roll_limit) {
            s_matrix.rotation3f.z = roll_limit;
        } else if (s_matrix.rotation3f.z < -roll_limit) {
            s_matrix.rotation3f.z = -roll_limit;
        }

        return 0;
    }, true));

    this->create_basic_models();

    for (feature* &object : this->object_list) {
        object->on_enable();
        object->on_reload();
    }

    effects::object sun_obj;

    sun_obj.flare = 128;
    sun_obj.position = glm::vec3(20, 20, 20);
    sun_obj.type = effects::objecttype::LIGHT;
    sun_obj.kd = glm::vec3(1, 1, 1);

    effects::add_world_objects(sun_obj);
}

void impl_editor_scene::on_disable() {
    feature::on_disable();

    for (feature* &object : this->object_list) {
        object->on_disable();
    }
}

void impl_editor_scene::on_event(SDL_Event &sdl_event) {
    feature::on_event(sdl_event);

    for (feature* &object : this->object_list) {
        object->on_event(sdl_event);
    }

    if (ui::event_listener()) {
        auto val = ui::event_poll();

        switch (val->type) {
            case ui::type::slider: {
                break;
            }

            case ui::type::button: {
                if (val->tag == "Exit") {
                    SDL_Event sdl_custom_event;
                    sdl_custom_event.type = SDL_QUIT;
                    SDL_PushEvent(&sdl_custom_event);
                }

                break;
            }

            case ui::type::slider3d: {
                auto slider3d = (ui::number*) val;

                if (slider3d->tag == "D-PAD") {
                    this->dpad_velocity.x = -slider3d->get_x() / this->setting_dpad_speed_factor->get_x();
                    this->dpad_velocity.z = -slider3d->get_y() / this->setting_dpad_speed_factor->get_x();
                }

                break;
            }
        }
    }

    if (input::event["Mouse2"] || input::event["Mouse1"]) {
        client::core.camera.set_camera_focused(true);
    } else if (client::core.camera.mouse_locked) {
        client::core.camera.set_camera_focused(false);
    }

    if (input::event["Mouse Scroll Up"]) {
        this->camera_direction.z += 5 * input::mousewheel.y;
    } else if (input::event["Mouse Scroll Down"]) {
        this->camera_direction.z += 5 * input::mousewheel.y;
    }

    if ((input::event["Mouse1"] && input::event["Mouse Motion"] && client::core.camera.mouse_locked) || (input::event["Mouse3"] && client::core.widget_service.focused_widget_id == 0)) {
        this->camera_direction.x += input::mouse.z * this->camera_speed * this->camera_speed;
        this->camera_direction.y += input::mouse.w * this->camera_speed * this->camera_speed;
    }
}

void impl_editor_scene::on_update() {
    feature::on_update();

    client::core.world_object.objects[0].position = client::core.camera.position;
    client::core.world_object.should_update = true;

    if (this->enable_camera_movement) {
        input::key_movement(this->camera_direction);
        this->apply_movement_physics();
    }

    if (input::event["Mouse Motion"]) {
        this->superior->model_plane.get_default_matrix().rotation3f += this->dpad_velocity;
    }

    if (this->dpad_velocity.x != 0.0f && this->dpad_velocity.z != 0.0f) {
        client::core.module_service.callback("refresh-pistons-angle");
    }

    for (feature* &object : this->object_list) {
        object->on_update();
    }
}

feature* impl_editor_scene::get_object_by_tag(const std::string &tag) {
    for (feature* &objects : this->object_list) {
        if (objects->tag == tag) {
            return objects;
        }
    }

    return nullptr;
}

void impl_editor_scene::on_render() {
    feature::on_render();

    for (feature* &object : this->object_list) {
        object->on_render();
    }

    this->batch.invoke();
    gpu::font.render("XYZ [" + std::to_string(client::core.camera.position.x) + ", " + std::to_string(client::core.camera.position.y) + ", " + std::to_string(client::core.camera.position.z) + "]", 1, input::screen::height - gpu::font.get_text_height() - 1, amogpu::vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    this->batch.revoke();
    this->batch.draw();
}

void impl_editor_scene::apply_movement_physics() {
    float f = client::core.camera.yaw;
    float x = glm::cos(glm::radians(f));
    float z = glm::sin(glm::radians(f));

    this->camera_velocity.x = this->camera_direction.z * this->camera_speed * x + this->camera_direction.x * this->camera_speed * z;
    this->camera_velocity.z = this->camera_direction.z * this->camera_speed * z - this->camera_direction.x * this->camera_speed * x;
    this->camera_velocity.y = this->camera_direction.y * this->camera_speed;

    client::core.camera.position += this->camera_velocity * input::deltatime;
    this->camera_direction = glm::vec3(0, 0, 0);
}

impl_editor_scene::impl_editor_scene() {
    this->tag = "EditorScene";
}

impl_editor_scene::~impl_editor_scene() {

}

void impl_editor_scene::create_basic_models() {
    for (feature* objects : this->object_list) {
        if (objects == nullptr) {
            continue;
        }

        delete objects;
    }

    ui::category("Virtual JoyStick")->initial_dock = ui::dock::right;
    this->setting_dpad_speed_factor = ui::slider(1.0f, 5.0f, "Speed Reduction");
    this->setting_dpad_speed_factor->set_x(2.666f);

    auto dpad = ui::slider3d("D-PAD", 1.0f);
    dpad->set_rollback_flag(true);
    dpad->set_x(1);
    dpad->set_y(1);

    ui::end_group();
    ui::category("Misc", {30, 30, 125, 30})->initial_dock = ui::dock::right;

    this->setting_max_roll = ui::slider(0.0f, 27.0f, "Max Roll");
    this->setting_max_yaw = ui::slider(0.0f, 20.0f, "Max Yaw");

    this->setting_max_roll->set_x(25.0f);
    this->setting_max_yaw->set_x(5.0f);

    ui::label("Piston 1:", ui::place::axis);
    auto p1 = ui::slider(0, 1.0f, "", ui::place::axis);
    p1->extra_tag = "piston0";

    ui::label("Piston 2:", ui::place::next);
    ui::slider(0, 1.0f, "", ui::place::axis)->extra_tag = "piston1";
    ui::label("Piston 3:", ui::place::next);
    ui::slider(0, 1.0f, "", ui::place::axis)->extra_tag = "piston2";
    ui::label("Piston 4:", ui::place::next);
    ui::slider(0, 1.0f, "", ui::place::axis)->extra_tag = "piston3";
    ui::label("Piston 5:", ui::place::next);
    ui::slider(0, 1.0f, "", ui::place::axis)->extra_tag = "piston4";
    ui::label("Piston 6:", ui::place::next);
    ui::slider(0, 1.0f, "", ui::place::axis)->extra_tag = "piston5";
    ui::end_group();

    this->object_list.clear();

    this->superior = new impl_stewart_platform_object("superior");
    this->inferior = new impl_stewart_platform_object("inferior");

    this->object_list.push_back(new impl_wireframe_object("wireframe"));
    this->object_list.push_back(this->superior);
    this->object_list.push_back(this->inferior);

    this->object_list.push_back(this->the_piston0 = new impl_piston_object("piston0"));
    this->object_list.push_back(this->the_piston1 = new impl_piston_object("piston1"));
    this->object_list.push_back(this->the_piston2 = new impl_piston_object("piston2"));
    this->object_list.push_back(this->the_piston3 = new impl_piston_object("piston3"));
    this->object_list.push_back(this->the_piston4 = new impl_piston_object("piston4"));
    this->object_list.push_back(this->the_piston5 = new impl_piston_object("piston5"));

    ui::category("Render")->initial_dock = ui::dock::left;

    ui::label("Superior platform:");
    auto superior_red = ui::slider(0.0f, 255.0f, "R", ui::place::axis);
    auto superior_green = ui::slider(0.0f, 255.0f, "G", ui::place::axis);
    auto superior_blue = ui::slider(0.0f, 255.0f, "B", ui::place::axis);

    ui::label("Inferior platform:", ui::place::fill);
    auto inferior_red = ui::slider(0.0f, 255.0f, "R", ui::place::axis);
    auto inferior_green = ui::slider(0.0f, 255.0f, "G", ui::place::axis);
    auto inferior_blue = ui::slider(0.0f, 255.0f, "B", ui::place::axis);

    ui::label("Pistons:", ui::place::fill);
    auto piston_red = ui::slider(0.0f, 255.0f, "R", ui::place::axis);
    auto piston_green = ui::slider(0.0f, 255.0f, "G", ui::place::axis);
    auto piston_blue = ui::slider(0.0f, 255.0f, "B", ui::place::axis);

    ui::button("Exit", ui::place::fill);
    ui::end_group();

    piston_red->set_x(100);
    piston_green->set_x(100);
    piston_blue->set_x(100);

    superior_red->set_x(200);
    superior_green->set_x(200);
    superior_blue->set_x(200);

    inferior_red->set_x(150);
    inferior_green->set_x(150);
    inferior_blue->set_x(150);

    ui::category("Platform", {30, 30, 300, 30})->initial_dock = ui::dock::left;

    this->superior->color_red = superior_red;
    this->superior->color_green = superior_green;
    this->superior->color_blue = superior_blue;
    this->superior->detail_angles = ui::slider3d("Superior Yaw & Roll", 360.0f);

    this->inferior->color_red = inferior_red;
    this->inferior->color_green = inferior_green;
    this->inferior->color_blue = inferior_blue;
    this->inferior->detail_angles = ui::slider3d("Inferior Yaw & Roll", 360.0f);

    ui::end_group();

    ui::number* val_number {};
    int32_t object_tokens {};

    // Spam tokens (whatever run some ticks more).
    for (int32_t tokens = 0; tokens < 12; tokens++) {
        auto object_tag = "piston" + std::to_string(object_tokens);
        auto piston = (impl_piston_object*) this->get_object_by_tag(object_tag);

        if (piston == nullptr) {
            continue;
        }

        if (ui::data[p1->data.id + tokens] == nullptr) {
            continue;
        }

        /*
         If you look in the built GUI, the sliders are close of labels.
         Piston 1: [SLIDER 1] // First element is a label, so we bypass it.
         Piston 2: [SLIDER 2] // Next... next...
         */
        if (ui::data[p1->data.id + tokens]->extra_tag != object_tag) {
            continue;
        }

        val_number = (ui::number*) ui::data[p1->data.id + tokens];
        piston->force = val_number;

        piston->color_red = piston_red;
        piston->color_green = piston_green;
        piston->color_blue = piston_blue;

        object_tokens++;
    }

    this->superior->set_scale(glm::vec3(9, 1, 9));
    this->inferior->set_scale(glm::vec3(10, 1, 10));
    this->superior->set_pos(glm::vec3(0, 12, 0));

    client::core.module_service.callback("reset-pistons");
    client::core.module_service.callback("refresh-pistons-angle");
}
