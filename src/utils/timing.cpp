#include "timing.hpp"
#include <SDL2/SDL.h>

bool timing::reach(timing::clock &clock, uint64_t reach_ms) {
    clock.concurrent_ticks = SDL_GetTicks();
    clock.ticks_going_on = clock.concurrent_ticks - clock.elapsed_ticks;

    if (clock.ticks_going_on >= reach_ms) {
        clock.elapsed_ticks = clock.concurrent_ticks;
        return true;
    }

    return false;
}