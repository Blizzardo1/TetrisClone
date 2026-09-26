#include "board.h"
#include "piece.h"
#include "timer.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define LOCK_DELAY 500 // ms

Uint32 deltatime;
Uint32 lasttime;
Uint32 locktime;

void timer_init(void) {
    srand(time(NULL));
}

int timer_get_rand(int min, int max) {
    assert(max >= min);
    unsigned int range = (unsigned int)(max - min) + 1;
    unsigned int limit = RAND_MAX - (RAND_MAX % range);
    unsigned int r;
    do {
        r = (unsigned int)rand();
    } while(r >= limit);
    return min + (int)(r % range);
}

Uint32 timer_get_lasttime(void) {
    return lasttime;
}

Uint32 timer_get_deltatime(void) {
    return deltatime;
}

int SDLCALL timer_update(void *arg) {
    (void)arg;
    Uint32 currenttime = SDL_GetTicks();
    deltatime = currenttime - lasttime;
    lasttime = currenttime;
    locktime += deltatime;
    if(locktime > LOCK_DELAY) {
        // int *width = get_board_width();
        // int *height = get_board_height();
        // char (*board)[BOARD_WIDTH] = get_board();
        // piece_move(piece_get_current(), 0, 1, board, *width, *height);
        locktime = 0;
    }
    return 0;
}