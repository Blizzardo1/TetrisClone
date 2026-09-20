#ifndef PIECE_H
#define PIECE_H

#include <SDL3/SDL.h>
#include "board.h"

#define SPAWN_CENTER_X (BOARD_WIDTH / 2)
#define SPAWN_CENTER_Y 2
#define TETROMINO_BLOCK_SIZE 32

typedef struct {
    char *name;
    char block[4][4];
    SDL_Color color;
    int x, y;
} Tetromino;


// #FFA200FF Orange J
// #0000FFFF Blue   L
// #00EEFFFF Cyan   I
// #FAF000FF Yellow O
// #00FF00FF Green  S
// #FF0000FF Red    Z
// #8300D4FF Purple T


extern Tetromino J;
extern Tetromino L;
extern Tetromino I;
extern Tetromino O;
extern Tetromino S;
extern Tetromino Z;
extern Tetromino T;

/**
 * @brief Initializes the first Tetromino.
 *
 * @param board_width the board width.
 * @param board_height the board height.
 */
void piece_init(int board_width, int board_height);

/**
 * @brief Centers the Tetromino to the top-center of the board.
 *
 * @param bx the X-Position of the board.
 * @param by the Y-Position of the board.
 * @param board_width the board width.
 */
void piece_center_tetromino(int bx, int by, int board_width);

/**
 * @brief Copies a Tetromino.
 *
 * @param tetromino the Tetromino to copy.
 * @return Tetromino* the copied Tetromino
 */
Tetromino* piece_tetromino_copy(Tetromino *tetromino);

/**
 * @brief Gets a random Tetromino.
 *
 * @return Tetromino* a newly chosen Tetromino.
 */
Tetromino* piece_get_random(void);

/**
 * @brief Gets the current Tetromino.
 *
 * @return Tetromino* the currently set Tetromino.
 */
Tetromino* piece_get_current(void);

/**
 * @brief Gets the next Tetromino.
 *
 * @return Tetromino* the currently queued Tetromino.
 */
Tetromino* piece_get_next(void);

/**
 * @brief Checks whether a Tetromino can move or not.
 * @param tetromino the Tetromino to check.
 *
 * @param dx destination X.
 * @param dy destination Y.
 * @param board the backend board.
 * @param board_width the width of the board.
 * @param board_height the height of the board.
 *
 * @returns true if the Tetromino can still move; otherwise false.
 */
bool piece_can_move(Tetromino *tetromino, int dx, int dy, char (*board)[BOARD_WIDTH], int board_width, int board_height);

/**
 * @brief Moves a Tetromino in any given direction.
 *
 * @param tetromino the Tetromino to move.
 * @param px the X-Position to move.
 * @param py the Y-Position to move.
 * @param board_width the board width.
 * @param board_height the board height.
 */
void piece_move(Tetromino *tetromino, int px, int py, char (*board)[BOARD_WIDTH], int board_width, int board_height);

/**
 * @brief Rotates a Tetromino.
 *
 * @param tetromino the Tetromino to rotate.
 */
void piece_rotate(Tetromino *tetromino);

/**
 * @brief Spawns a Tetromino
 *
 * @param board_width the board width.
 * @param board_height the board height.
 */
void piece_spawn(int board_width);

/**
 * @brief Draws a Tetromino.
 *
 * @param tetromino the Tetromino to draw.
 * @param px the X-Position to draw.
 * @param py the Y-Position to draw.
 */
void piece_draw(Tetromino *tetromino, int px, int py);

#endif