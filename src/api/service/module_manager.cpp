#include "module_manager.hpp"
#include "utils/log.hpp"

module_task::module_task(const std::string &the_tag, feature* callback_feature, std::function<int(feature*)> callable_fun, bool initial_state) {
    this->tag = the_tag;
    this->main = callback_feature;
    this->is_enabled = initial_state;
    this->callable = callable_fun;
}

module_task::~module_task() {

}

void module_manager::registry(module_task* task) {
    if (task == nullptr) {
        return;
    }

    this->module_task_list.push_back(task);
    log_message("-- Module service register: Module task " + task->tag);
}

void module_manager::callback(const std::string &tag) {
    for (module_task* &task : this->module_task_list) {
        if (task->tag == tag) {
            task->is_enabled = true;
            this->should_refresh_task_list = true;
            break;
        }
    }
}

void module_manager::registry(feature *module) {
    if (module == nullptr) {
        return;
    }

    this->feature_list.push_back(module);
    log_message("-- Module service register: Module " + module->tag);
}

feature *module_manager::get_feature_by_tag(const std::string &tag) {
    for (feature* &modules : this->feature_list) {
        if (modules->tag == tag) {
            return modules;
        }
    }

    return nullptr;
}

void module_manager::on_event(SDL_Event &sdl_event) {
    for (feature* &modules : this->feature_list) {
        if (modules->state) {
            modules->on_event(sdl_event);
        }
    }
}

void module_manager::on_update() {
    for (feature* &modules : this->feature_list) {
        if (modules->state) {
            modules->on_update();
        }
    }

    if (this->should_refresh_task_list) {
        for (module_task* &task : this->module_task_list) {
            if (task->is_enabled) {
                task->is_enabled = task->callable(task->main);
            }
        }

        this->should_refresh_task_list = false;
    }
}

void module_manager::on_render() {
    for (feature* &modules : this->feature_list) {
        if (modules->state) {
            modules->on_render();
        }
    }
}

void module_manager::on_init() {
    for (feature* &modules : this->feature_list) {
        modules->on_enable();
    }
}

void module_manager::on_quit() {
    for (feature* &modules : this->feature_list) {
        modules->on_disable();
    }
}
