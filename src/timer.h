#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>

void timer_init();
int timer_get_rand(int min, int max);


#endif