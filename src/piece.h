#ifndef PIECE_H
#define PIECE_H

#include <SDL3/SDL.h>
#include <stdlib.h>
#include "timer.h"
#include "board.h"

#define SPAWN_CENTER_X 4
#define SPAWN_CENTER_Y 2
#define TETROMINO_BLOCK_SIZE 32

typedef struct {
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

void piece_init(int board_width, int board_height);
void piece_center_tetromino(int board_width, int board_height);
Tetromino* piece_tetromino_copy(Tetromino *tetromino);
Tetromino* piece_get_random(void);
Tetromino* piece_get_current(void);
Tetromino* piece_get_next(void);
void piece_spawn(int board_width, int board_height);

#endif