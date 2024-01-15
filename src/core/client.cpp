#include "client.hpp"

activy_core client::core {};

render::theme &client::theme() {
    return client::core.widget_service.theme;
}

bool client::change(bool &val, bool new_val) {
    if (val != new_val) {
        val = new_val;
        client::core.widget_service.should_redraw = true;
    }

    return val;
}
