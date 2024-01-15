#include "impl_main_context.hpp"
#include "core/client.hpp"
#include "impl/editor/impl_editor_scene.hpp"

void impl_main_context::on_create() {
    context::on_create();

    client::core.module_service.registry(new impl_editor_scene());
    client::core.module_service.registry(new module_task("refresh-screen", nullptr, [](feature* main) {
        SDL_SetWindowSize(client::core.sdl_window, 1280, 800);
        client::core.widget_service.should_force_fix_stack = true;
        client::core.widget_service.should_update = true;
        return 0;
    }));
}

void impl_main_context::on_destroy() {
    context::on_destroy();
}

void impl_main_context::on_event(SDL_Event &sdl_event) {
    context::on_event(sdl_event);
}

void impl_main_context::on_update() {
    context::on_update();

    if (this->ticks_going_on_patch < 2) {
        this->ticks_going_on_patch++;
    } else if (this->ticks_going_on_patch != 61) {
        client::core.module_service.callback("refresh-screen");
        client::core.module_service.callback("refresh-pistons-angle");
        this->ticks_going_on_patch = 61;
    }
}

void impl_main_context::on_render() {
    context::on_render();
}

impl_main_context::impl_main_context() {

}

impl_main_context::~impl_main_context() {

}
