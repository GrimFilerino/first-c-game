#ifndef TIME_H
#define TIME_H

#include "../types.h"

typedef struct time_state {
    float delta;
    float now;
    float last;
    float frame_last;
    float frame_delay;
    float frame_time;
    u32 frame_rate;
    u32 frame_count;
} Time_State;

void time_init(u32 frame_rate);
void time_update();
void time_update_late();

#endif
