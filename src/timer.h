#ifndef TIMER_H
#define TIMER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>

/**
 * @brief Initializes the Timer.
 */
void timer_init();

/**
 * @brief Gets a random between min and max.
 * 
 * @param min the lowest number.
 * @param max the highest number.
 * @return int a random number within the range of min and max.
 */
int timer_get_rand(int min, int max);

/**
 * @brief Gets the last time.
 *
 * @return Uint32 the last time.
 */
Uint32 timer_get_lasttime(void);

/**
 * @brief Gets the delta time.
 * @remark This will capture the amount of time taken between each frame.
 *
 * @return Uint32 the delta time.
 */
Uint32 timer_get_deltatime(void);

/**
 * @brief Updates the timer.
 * @remarks This function will update the delta time and last time variables.
 *
 * @param arg unused. This is for SDL_Thread.
 */
int SDLCALL timer_update(void *arg);

#endif