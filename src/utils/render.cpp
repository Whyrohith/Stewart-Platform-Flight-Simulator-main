#include "render.hpp"
#include "core/client.hpp"

void render::process(const math::rect &rect, const amogpu::vec4f &color) {
    render::process(rect.x, rect.y, rect.w, rect.h, color);
}

void render::process(const std::string &text, float x, float y, const amogpu::vec4f &color, render::font font_size) {
    switch (font_size) {
        case render::font::small: {
            client::core.widget_service.f_render_size_small.render(text, x, y, color);
            break;
        }

        case render::font::normal: {
            client::core.widget_service.f_render_size_normal.render(text, x, y, color);
            break;
        }

        case render::font::high: {
            client::core.widget_service.f_render_size_high.render(text, x, y, color);
            break;
        }
    }
}

void render::update(SDL_Event &sdl_event) {

}

float render::get_text_width(const std::string &text, render::font font_size) {
    switch (font_size) {
        case render::font::small: {
            return client::core.widget_service.f_render_size_small.get_text_width(text);
        }

        case render::font::normal: {
            return client::core.widget_service.f_render_size_normal.get_text_width(text);
        }

        case render::font::high: {
            return client::core.widget_service.f_render_size_high.get_text_width(text);
        }
    }

    return 0;
}

float render::get_text_height(render::font font_size) {
    switch (font_size) {
        case render::font::small: {
            return client::core.widget_service.f_render_size_small.get_text_height();
        }

        case render::font::normal: {
            return client::core.widget_service.f_render_size_normal.get_text_height();
        }

        case render::font::high: {
            return client::core.widget_service.f_render_size_high.get_text_height();
        }
    }

    return 0;
}

void render::process(float x, float y, float w, float h, const amogpu::vec4f &color) {
    client::core.widget_service.batch.instance(x, y);
    client::core.widget_service.batch.fill(color);
    client::core.widget_service.batch.modal(w, h);

    x = 0;
    y = 0;
    w = 1;
    h = 1;

    client::core.widget_service.batch.vertex(x, y);
    client::core.widget_service.batch.vertex(x, y + h);
    client::core.widget_service.batch.vertex(x + w, y + h);
    client::core.widget_service.batch.vertex(x + w, y + h);
    client::core.widget_service.batch.vertex(x + w, y);
    client::core.widget_service.batch.vertex(x, y);
    client::core.widget_service.batch.coords(0.0f, 0.0f);
    client::core.widget_service.batch.coords(0.0f, 0.0f);
    client::core.widget_service.batch.coords(0.0f, 0.0f);
    client::core.widget_service.batch.coords(0.0f, 0.0f);
    client::core.widget_service.batch.coords(0.0f, 0.0f);
    client::core.widget_service.batch.coords(0.0f, 0.0f);
    client::core.widget_service.batch.next();
}
