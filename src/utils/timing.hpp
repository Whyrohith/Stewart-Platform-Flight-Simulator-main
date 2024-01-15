#pragma once
#ifndef SPF_TIMING_H
#define SPF_TIMING_H

#include <iostream>

namespace timing {
    struct clock {
        uint32_t elapsed_ticks {};
        uint32_t concurrent_ticks {};
        uint32_t ticks_going_on {};
    };

    bool reach(timing::clock &clock, uint64_t reach_ms);
}

#endif