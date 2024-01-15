#pragma once
#ifndef SPF_RENDER_H
#define SPF_RENDER_H

#include "math.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <amogpu/amogpu.hpp>

namespace render {
    struct theme {
        math::point dock_offset {5.0f, 5.0f};
        math::point parent_offset {2.0f, 2.0f};

        amogpu::vec4f string_frame {0.0f, 0.0f, 0.0f, 1.0f};
        amogpu::vec4f background_frame {0.6f, 0.6f, 0.6f, 1.0f};
        amogpu::vec4f activy_frame {0.5f, 0.4f, 0.8f, 1.0f};

        amogpu::vec4f string_button {0.0f, 0.0f, 0.0f, 1.0f};
        amogpu::vec4f background_button {1.0f, 1.0f, 1.0f, 0.3f};
        amogpu::vec4f activy_button {0.5f, 0.4f, 0.8f, 1.0f};
        amogpu::vec4f highlight_button {1.0f, 1.0f, 1.0f, 0.3f};

        amogpu::vec4f string_checkbox {0.0f, 0.0f, 0.0f, 1.0f};
        amogpu::vec4f background_checkbox {1.0f, 1.0f, 1.0f, 0.0f};
        amogpu::vec4f activy_checkbox {0.5f, 0.4f, 0.8f, 1.0f};
        amogpu::vec4f highlight_checkbox {1.0f, 1.0f, 1.0f, 0.3f};

        amogpu::vec4f string_slider {0.0f, 0.0f, 0.0f, 1.0f};
        amogpu::vec4f background_slider {1.0f, 1.0f, 1.0f, 0.3f};
        amogpu::vec4f activy_slider {0.5f, 0.4f, 0.8f, 1.0f};
        amogpu::vec4f highlight_slider {1.0f, 1.0f, 1.0f, 0.3f};

        amogpu::vec4f string_slider3d {0.0f, 0.0f, 0.0f, 1.0f};
        amogpu::vec4f background_slider3d {1.0f, 1.0f, 1.0f, 0.3f};
        amogpu::vec4f activy_slider3d {0.5f, 0.4f, 0.8f, 1.0f};
        amogpu::vec4f label_string {0.0f, 0.0f, 0.0f, 1.0f};
    };

    enum font {
        small, normal, high
    };

    float get_text_width(const std::string &text, render::font font_size = render::font::normal);
    float get_text_height(render::font font_size = render::font::normal);

    void process(const math::rect &rect, const amogpu::vec4f &color);
    void process(float x, float y, float w, float h, const amogpu::vec4f &color);
    void process(const std::string &text, float x, float y, const amogpu::vec4f &color, render::font font_size = render::font::normal);

    void update(SDL_Event &sdl_event);
};

#endif