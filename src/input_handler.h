#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_thread.h>
#include "tetris.h"
#include "piece.h"

#define HARD_DROP SDLK_SPACE
#define SOFT_DROP SDLK_DOWN
#define HOLD SDLK_C
#define ROTATE SDLK_UP
#define LEFT SDLK_LEFT
#define RIGHT SDLK_RIGHT
#define MENU SDLK_ESCAPE

void handle_init(void);
void handle_input(SDL_KeyboardEvent *event);
void handle_hard_drop(void);
void handle_soft_drop(void);
void handle_hold(void);
void handle_rotate(void);
void handle_left(void);
void handle_right(void);
void handle_menu(void);

#endif