#include "log.hpp"

std::string relative_path {};

void log_message(const std::string &input_str) {
    const std::string str = "[MAIN] " + input_str;
    std::cout << str.c_str() << '\n';
}