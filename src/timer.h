#ifndef TIMER_H
#define TIMER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>

void timer_init();
int timer_get_rand(int min, int max);
Uint32 timer_get_lasttime(void);
Uint32 timer_get_deltatime(void);
int SDLCALL timer_update(void *arg);

#endif