#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_thread.h>
#include "tetris.h"
#include "piece.h"


// Hardcoded keycodes for now
#define HARD_DROP SDLK_SPACE
#define SOFT_DROP SDLK_DOWN
#define HOLD SDLK_C
#define ROTATE SDLK_UP
#define LEFT SDLK_LEFT
#define RIGHT SDLK_RIGHT
#define MENU SDLK_ESCAPE


/**
 * @brief Initializes the Input Handler.
 */
void handle_init(void);

/**
 * @brief Handles each key press.
 *
 * @param event the captured key.
 */
void handle_input(SDL_KeyboardEvent *event);

/**
 * @brief Sends a Tetromino immediately down (hard drop).
 */
void handle_hard_drop(void);

/**
 * @brief Sends a Tetromino slightly down (soft drop).
 */
void handle_soft_drop(void);

/**
 * @brief Sends a Tetromino to the holding cell.
 */
void handle_hold(void);

/**
 * @brief Rotates a Tetromino
 * @remarks Only in one direction will the Tetromino spin.
 * There is no support yet for rotating the other way.
 */
void handle_rotate(void);

/**
 * @brief Moves the Tetromino Left by one Block.
 */
void handle_left(void);

/**
 * @brief Moves the Tetromino Right by one Block.
 */
void handle_right(void);

/**
 * @brief Shows a Menu.
 */
void handle_menu(void);

#endif