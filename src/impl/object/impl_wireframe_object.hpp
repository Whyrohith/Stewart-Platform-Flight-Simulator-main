#pragma once
#ifndef SPF_IMPL_WIREFRAME_OBJECT_H
#define SPF_IMPL_WIREFRAME_OBJECT_H

#include "api/model/model.hpp"
#include "api/feature/feature.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <SDL2/SDL.h>

class impl_wireframe_object : public feature {
public:
    model model_element {};
    int32_t range {};

    impl_wireframe_object(const std::string &tag_of) {
        this->tag = tag_of;
    }

    void on_reload();

    void on_enable() override;
    void on_disable() override;
    void on_event(SDL_Event &sdl_event) override;
    void on_render() override;
    void on_update() override;
};

#endif