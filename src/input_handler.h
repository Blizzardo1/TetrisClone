#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include "tetris.h"

#define HARD_DROP SDLK_SPACE
#define SOFT_DROP SDLK_DOWN
#define HOLD SDLK_C
#define ROTATE SDLK_UP
#define LEFT SDLK_LEFT
#define RIGHT SDLK_RIGHT
#define MENU SDLK_ESCAPE

void handle_input(SDL_KeyboardEvent *event);
void handle_hard_drop();
void handle_soft_drop();
void handle_hold();
void handle_rotate();
void handle_left();
void handle_right();
void handle_menu();

#endif