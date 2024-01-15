#pragma once
#ifndef SPF_LOG_H
#define SPF_LOG_H

#include <map>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

extern std::string relative_path;
void log_message(const std::string &input_str);

#endif