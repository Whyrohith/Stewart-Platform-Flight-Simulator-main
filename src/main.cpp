#include "core/client.hpp"
#include "impl/main/impl_main_context.hpp"
#include "utils/log.hpp"

#define VERSION "1.0.0-SNAPSHOT"

void fix_relative_path() {
    std::string token {"/"};
    auto end = relative_path.find(token);
    auto target = 0U;

    while (end != std::string::npos) {
        target = end;
        end = relative_path.find(token, end + token.length());
    }

    relative_path = relative_path.substr(0, target);
    relative_path += relative_path.empty() ? "" : "/";

    log_message("Path: " + relative_path);
}

int main(int argv, char** argc) {
    log_message(VERSION);
    relative_path = argc[0];
    fix_relative_path();

    client::core.make_current(new impl_main_context());
    client::core.init();
    client::core.run();
    client::core.shutdown();

    return 0;
}
