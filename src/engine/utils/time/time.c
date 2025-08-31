#include <SDL3/SDL.h>
#include "time.h"
#include "../../engine.h"
#include "SDL3/SDL_timer.h"

void time_init(u32 frame_rate){
    engine.time.frame_rate = frame_rate;
    engine.time.frame_delay = 1000.f / frame_rate;
}

void time_update() {
    engine.time.now = (float)SDL_GetTicks();
    engine.time.delta = (engine.time.now - engine.time.last) / 1000.f;
    engine.time.last = engine.time.now;

    if(engine.time.now - engine.time.frame_last >= 1000.f) {
        engine.time.frame_rate = engine.time.frame_count;
        engine.time.frame_count = 0;
        engine.time.frame_last = engine.time.now;
    }
}

void time_update_late() {
    engine.time.frame_time = (float)SDL_GetTicks() - engine.time.now;
    
    if(engine.time.frame_delay > engine.time.frame_time) {
        SDL_Delay(engine.time.frame_delay - engine.time.frame_time);
    }
}
