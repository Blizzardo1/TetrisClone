#include "timer.h"
#include <assert.h>

void timer_init() {
    srand(time(NULL));
}

int get_rand(int min, int max) {
    assert(max >= min);
    unsigned int range = (unsigned int)(max - min) + 1;
    unsigned int limit = RAND_MAX - (RAND_MAX % range);
    unsigned int r;
    do {
        r = (unsigned int)rand();
    } while(r >= limit);
    return min + (int)(r % range);
}