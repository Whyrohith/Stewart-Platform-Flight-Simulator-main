#pragma once
#ifndef SPF_CLIENT_H
#define SPF_CLIENT_H

#include "core.hpp"

namespace client {
    extern activy_core core;
    render::theme &theme();
    bool change(bool &val, bool new_val);
}

#endif