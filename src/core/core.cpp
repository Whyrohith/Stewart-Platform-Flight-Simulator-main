#include "core.hpp"
#include "utils/log.hpp"
#include "utils/input.hpp"
#include <GL/glew.h>
#include "api/world/effects.hpp"
#include "amogpu/amogpu.hpp"

void activy_core::init() {
    log_message("Initialising video");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    input::screen::width  = 1280;
    input::screen::height = 800;

    this->sdl_window = SDL_CreateWindow("Stewart Platform - Flight Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    this->sdl_gl_context = SDL_GL_CreateContext(this->sdl_window);

    glewExperimental = true;
    glewInit();

    log_message("-- GPU device:");

    if (glGetString(GL_VENDOR) || glGetString(GL_RENDERER)) {
        log_message(std::string((const char*) glGetString(GL_VENDOR)));
        log_message(std::string((const char*) glGetString(GL_RENDERER)));
    } else {
        log_message("No GPU device information.");
    }

    log_message("-- OpenGL context:");

    if (glGetString(GL_VERSION)) {
        log_message("OpenGL " + std::string((const char*) glGetString(GL_VERSION)));
    } else {
        log_message("No GL context information.");
    }

    SDL_GL_SetSwapInterval(1);

    amogpu::gl_version = "#version 450 core";
    amogpu::init();
    
    effects::load();
    gpu::init();
}

void activy_core::shutdown() {
    this->module_service.on_quit();
    this->widget_service.on_quit();

    log_message("Shutdown complete");
}

void activy_core::run() {
    this->module_service.on_init();
    this->widget_service.on_init();
    this->camera.on_init();

    this->interval_fps = 16;
    this->running = true;

    SDL_Event sdl_event;
    glEnable(GL_DEPTH_TEST);

    while (this->running) {
        if (timing::reach(this->loop_clock, this->interval_fps)) {
            input::deltatime = static_cast<float>(this->loop_clock.ticks_going_on) / 100;

            if (timing::reach(this->fps_clock, 1000)) {
                this->fps = this->elapsed_frame;
                this->elapsed_frame = 0;
            }

            this->on_event(sdl_event);
            this->on_update();
            this->on_render();

            this->elapsed_frame++;
            SDL_GL_SwapWindow(this->sdl_window);
        }
    }
}

void activy_core::on_event(SDL_Event &sdl_event) {
    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            case SDL_QUIT: {
                this->running = false;
                break;
            }

            default: {
                input::update(sdl_event);
                this->widget_service.on_event(sdl_event);

                if (this->activy_context != nullptr) {
                    this->activy_context->on_event(sdl_event);
                }

                this->module_service.on_event(sdl_event);
                this->camera.on_event(sdl_event);

                break;
            }
        }
    }
}

void activy_core::on_update() {
    this->world_object.update();

    if (this->activy_context != nullptr) {
        this->activy_context->on_update();
    }

    this->module_service.on_update();
    this->widget_service.on_update();
}

void activy_core::on_render() {
    glViewport(0, 0, static_cast<int32_t>(input::screen::width), static_cast<int32_t>(input::screen::height));
    amogpu::matrix();
    this->camera.on_update_matrix();

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    if (this->activy_context != nullptr) {
        this->activy_context->on_render();
    }

    this->module_service.on_render();
    this->widget_service.on_render();
}

void activy_core::make_current(context *raw_context) {
    if (raw_context == nullptr) {
        if (this->activy_context != nullptr) {
            this->activy_context->on_destroy();
            delete this->activy_context;
            this->activy_context = nullptr;
        }

        return;
    }

    if (this->activy_context == nullptr) {
        this->activy_context = raw_context;
        this->activy_context->on_create();
        return;
    }

    if (raw_context->tag == this->activy_context->tag) {
        delete raw_context;
    } else {
        this->activy_context->on_destroy();
        delete this->activy_context;
        this->activy_context = raw_context;
        this->activy_context->on_create();
    }
}
