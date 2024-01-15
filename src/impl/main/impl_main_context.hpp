#pragma once
#ifndef SPF_IMPL_MAIN_CONTEXT_H
#define SPF_IMPL_MAIN_CONTEXT_H

#include "api/context/context.hpp"

class impl_main_context : public context {
public:
    impl_main_context();
    ~impl_main_context();

    int32_t ticks_going_on_patch {};

    void on_create() override;
    void on_destroy() override;
    void on_event(SDL_Event &sdl_event) override;
    void on_update() override;
    void on_render() override;
};

#endif