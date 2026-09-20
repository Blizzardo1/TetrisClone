#include <stdlib.h>
#include "color.h"
#include "timer.h"
#include "piece.h"

#define D '.'
#define N ' '

Tetromino J = {
    .name = "J",
    .block = {
        {N,N,N,N},
        {N,N,D,N},
        {N,N,D,N},
        {N,D,D,N},
    },
    .color = {0,0,255,255} // Blue
};

Tetromino L = {
    .name = "L",
    .block = {
        {N,N,N,N},
        {N,D,N,N},
        {N,D,N,N},
        {N,D,D,N},
    },
    .color = {255,162,0,255} // Orange
};

Tetromino I = {
    .name = "I",
    .block = {
        {N,N,D,N},
        {N,N,D,N},
        {N,N,D,N},
        {N,N,D,N},
    },
    .color = {0,238,255,255} // Cyan
};

Tetromino O = {
    .name = "O",
    .block = {
        {N,N,N,N},
        {N,N,N,N},
        {N,D,D,N},
        {N,D,D,N},
    },
    .color = {250,240,0,255} // Yellow
};

Tetromino S = {
    .name = "S",
    .block = {
        {N,N,N,N},
        {N,N,D,D},
        {N,D,D,N},
        {N,N,N,N},
    },
    .color = {0,255,0,255} // Green
};

Tetromino Z = {
    .name = "Z",
    .block = {
        {N,N,N,N},
        {D,D,N,N},
        {N,D,D,N},
        {N,N,N,N},
    },
    .color = {255,0,0,255} // Red
};

Tetromino T = {
    .name = "T",
    .block = {
        {N,N,N,N},
        {N,N,D,N},
        {N,D,D,D},
        {N,N,N,N},
    },
    .color = {131,0,212,255} // Purple
};


Tetromino *current;
Tetromino *next;

void piece_init(int board_width, int board_height) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing Piece");
    next = piece_get_random();
    piece_spawn(board_width, board_height);
}

void piece_center_tetromino(int board_width, int board_height) {
    // SPAWN_CENTER_X * ((board_width / 2))
    // (1920/2) * ()
    current->x = SPAWN_CENTER_X * ((board_width / 2) * (TETROMINO_BLOCK_SIZE / 2));
    current->y = SPAWN_CENTER_Y * ((board_height  / 2) * (TETROMINO_BLOCK_SIZE / 2));
    SDL_Log("%s spawned: BW=%d, BH=%d, XY (%d, %d)", current->name, board_width, board_height, current->x, current->y);
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


bool piece_can_move(Tetromino *tetromino, int dx, int dy, char (*board)[BOARD_WIDTH], int board_width, int board_height) {
    int nX = tetromino->x + dx;
    int nY = tetromino->y + dy;

    for(int row = 0; row < board_height; row++) {
        for (int col = 0; col < board_width; col++) {
            if(tetromino->block[row][col] == N) {
                continue;
            }
            int bx = nX + col;
            int by = nY + row;
            if (bx < 0 || bx >= board_width || by >= board_height) {
                return false;
            }
            if(by < 0) {
                continue;
            }
            if(board[by][bx] != N) {
                return false;
            }
        }
    }
    return true;
}

void piece_move(Tetromino *tetromino, int px, int py, char (*board)[BOARD_WIDTH], int board_width, int board_height) {
    if(py == -1) {
        // Hard Drop
        while(piece_can_move(tetromino, px, 1, board, board_width, board_height)) {
            tetromino->y += py * TETROMINO_BLOCK_SIZE;
        }
    }
    if(!piece_can_move(tetromino, px, py, board, board_width, board_height)) {
        return;
    }
    tetromino->x += px * TETROMINO_BLOCK_SIZE;
    tetromino->y += py * TETROMINO_BLOCK_SIZE;
}

void piece_rotate(Tetromino *tetromino) {
    char temp[4][4];
    for(int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            temp[x][3 - y] = tetromino->block[y][x];
        }
    }
    memcpy(tetromino->block, temp, sizeof(tetromino->block));
}

void piece_spawn(int board_width, int board_height) {
    current = next;
    piece_center_tetromino(board_width, board_height);
    next = piece_get_random();
}

void piece_draw(Tetromino *tetromino, int px, int py) {
    SDL_Renderer *renderer = get_renderer();
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if (tetromino->block[y][x] == ' ') {
                continue;
            }
            SDL_FRect rect = {
                px + (x * TETROMINO_BLOCK_SIZE),
                py + (y * TETROMINO_BLOCK_SIZE),
                TETROMINO_BLOCK_SIZE,
                TETROMINO_BLOCK_SIZE
            };
            set_draw_color(tetromino->color);
            SDL_RenderFillRect(renderer, &rect);
            set_draw_color(BLACK);
            SDL_RenderRect(renderer, &rect);
        }
    }
}
