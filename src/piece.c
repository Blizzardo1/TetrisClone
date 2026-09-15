#include "piece.h"



Tetromino J = {
    .block = {
        {' ', ' ', ' ', ' '},
        {' ', ' ', '.', ' '},
        {' ', ' ', '.', ' '},
        {' ', '.', '.', ' '},
    },
    .color = {0,0,255,255} // Blue
};

Tetromino L = {
    .block = {
        {' ', ' ', ' ', ' '},
        {' ', '.', ' ', ' '},
        {' ', '.', ' ', ' '},
        {' ', '.', '.', ' '},
    },
    .color = {255,162,0,255} // Orange
};

Tetromino I = {
    .block = {
        {' ', ' ', '.', ' '},
        {' ', ' ', '.', ' '},
        {' ', ' ', '.', ' '},
        {' ', ' ', '.', ' '},
    },
    .color = {0,238,255,255} // Cyan
};

Tetromino O = {
    .block = {
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '},
        {' ', '.', '.', ' '},
        {' ', '.', '.', ' '},
    },
    .color = {250,240,0,255} // Yellow
};

Tetromino S = {
    .block = {
        {' ', ' ', ' ', ' '},
        {' ', ' ', '.', '.'},
        {' ', '.', '.', ' '},
        {' ', ' ', ' ', ' '},
    },
    .color = {0,255,0,255} // Green
};

Tetromino Z = {
    .block = {
        {' ', ' ', ' ', ' '},
        {'.', '.', ' ', ' '},
        {' ', '.', '.', ' '},
        {' ', ' ', ' ', ' '},
    },
    .color = {255,0,0,255} // Red
};

Tetromino T = {
    .block = {
        {' ', ' ', ' ', ' '},
        {' ', ' ', '.', ' '},
        {' ', '.', '.', '.'},
        {' ', ' ', ' ', ' '},
    },
    .color = {131,0,212,255} // Purple
};


Tetromino *current;
Tetromino *next;

void piece_init(int board_width, int board_height) {
    next = piece_get_random();
    piece_spawn(board_width, board_height);
}

void piece_center_tetromino(int board_width, int board_height) {
    current->x = SPAWN_CENTER_X * ((board_width / 2) *  (TETROMINO_BLOCK_SIZE / 2));
    current->y = SPAWN_CENTER_Y * ((board_height  / 2) * (TETROMINO_BLOCK_SIZE / 2));
}

Tetromino* piece_tetromino_copy(Tetromino *tetromino) {
    Tetromino *t = malloc(sizeof(Tetromino));
    memcpy(t, tetromino, sizeof(Tetromino));
    return t;
}

Tetromino* piece_get_random() {
    int rand = timer_get_rand(0, 6);
    switch(rand) {
        case 0: return piece_tetromino_copy(&J);
        case 1: return piece_tetromino_copy(&L);
        case 2: return piece_tetromino_copy(&I);
        case 3: return piece_tetromino_copy(&O);
        case 4: return piece_tetromino_copy(&S);
        case 5: return piece_tetromino_copy(&T);
        case 6: return piece_tetromino_copy(&Z);
    }
    // Should never be able to return a null Tetromino
    return NULL;
}

Tetromino* piece_get_current() {
    return current;
}

Tetromino* piece_get_next() {
    return next;
}

void piece_spawn(int board_width, int board_height) {
    current = next;
    piece_center_tetromino(board_width, board_height);
    next = piece_get_random();
}