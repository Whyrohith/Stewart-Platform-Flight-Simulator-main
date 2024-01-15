#pragma once
#ifndef SPF_MODULE_MANAGER_H
#define SPF_MODULE_MANAGER_H

#include "api/feature/feature.hpp"
#include <vector>
#include <functional>

struct module_task {
    std::function<int(feature*)> callable {};
    std::string tag {};

    feature* main {nullptr};
    bool is_enabled {false};

    module_task(const std::string &the_tag, feature* callback_feature, std::function<int(feature*)> callable_fun, bool initial_state = false);
    ~module_task();
};

class module_manager {
protected:
    std::vector<feature*> feature_list;
    std::vector<module_task*> module_task_list;
public:
    bool should_refresh_task_list {true};

    void registry(feature* module);
    feature* get_feature_by_tag(const std::string &tag);

    void registry(module_task* task);
    void callback(const std::string &tag);

    void on_init();
    void on_quit();
    void on_event(SDL_Event &sdl_event);
    void on_update();
    void on_render();
};

#endif