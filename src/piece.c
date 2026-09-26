#include <stdlib.h>
#include "color.h"
#include "timer.h"
#include "piece.h"

// The following Tetrominos must be set with a constant
// color and cannot use the MACRO defined colors.
Tetromino J = {
    .name = "J",
    .block = {
        {' ',' ',' ',' '},
        {' ',' ','J',' '},
        {' ',' ','J',' '},
        {' ','J','J',' '},
    },
    .color = {0,0,255,255} // Blue
};

Tetromino L = {
    .name = "L",
    .block = {
        {' ',' ',' ',' '},
        {' ','l',' ',' '},
        {' ','l',' ',' '},
        {' ','l','l',' '},
    },
    .color = {255,162,0,255} // Orange
};

Tetromino I = {
    .name = "I",
    .block = {
        {' ',' ','i',' '},
        {' ',' ','i',' '},
        {' ',' ','i',' '},
        {' ',' ','i',' '},
    },
    .color = {0,238,255,255} // Cyan
};

Tetromino O = {
    .name = "O",
    .block = {
        {' ',' ',' ',' '},
        {' ',' ',' ',' '},
        {' ','o','o',' '},
        {' ','o','o',' '},
    },
    .color = {250,240,0,255} // Yellow
};

Tetromino S = {
    .name = "S",
    .block = {
        {' ',' ',' ',' '},
        {' ',' ','s','s'},
        {' ','s','s',' '},
        {' ',' ',' ',' '},
    },
    .color = {0,255,0,255} // Green
};

Tetromino Z = {
    .name = "Z",
    .block = {
        {' ',' ',' ',' '},
        {'z','z',' ',' '},
        {' ','z','z',' '},
        {' ',' ',' ',' '},
    },
    .color = {255,0,0,255} // Red
};

Tetromino T = {
    .name = "T",
    .block = {
        {' ',' ',' ',' '},
        {' ',' ','t',' '},
        {' ','t','t','t'},
        {' ',' ',' ',' '},
    },
    .color = {131,0,212,255} // Purple
};


Tetromino *current;
Tetromino *next;

void piece_init(int board_width, int board_height) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing Piece: BW: %d, BH: %d", board_width, board_height);
    next = piece_get_random();
    piece_spawn(board_width);
}

void piece_center_tetromino(int bx, int by, int board_width) {
    current->x = SPAWN_CENTER_X;
    current->y = SPAWN_CENTER_Y;
    SDL_Log("%s spawned: BX=%d, BY=%d, BW=%d, XY (%d, %d)", current->name, bx, by, board_width, current->x, current->y);
}

Tetromino* piece_tetromino_copy(Tetromino *tetromino) {
    Tetromino *t = malloc(sizeof(Tetromino));
    memcpy(t, tetromino, sizeof(Tetromino));
    return t;
}

Tetromino* piece_get_random(void) {
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

Tetromino* piece_get_current(void) {
    return current;
}

Tetromino* piece_get_next(void) {
    return next;
}


bool piece_can_move(Tetromino *tetromino, int dx, int dy, char (*board)[BOARD_WIDTH], int board_width, int board_height) {
    int nX = tetromino->x + dx;
    int nY = tetromino->y + dy;

    for(int row = 0; row < board_height; row++) {
        for (int col = 0; col < board_width; col++) {
            if(tetromino->block[row][col] == ' ') {
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
            if(board[by][bx] != ' ') {
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

void piece_spawn(int board_width) {
    current = next;
    SDL_Point board_loc = get_board_location();
    piece_center_tetromino(board_loc.x, board_loc.y, board_width);
    next = piece_get_random();
}

void piece_draw(Tetromino *tetromino, int px, int py) {
    SDL_Renderer *renderer = get_renderer();
    SDL_Point board_loc = get_board_location();
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if (tetromino->block[y][x] == ' ') {
                continue;
            }
            SDL_FRect rect = {
                board_loc.x + (px + (x * TETROMINO_BLOCK_SIZE)),
                board_loc.y + (py + (y * TETROMINO_BLOCK_SIZE)),
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

void piece_update(Tetromino *tetromino, int cx, int cy) {
    char (*board)[BOARD_WIDTH] = get_board();
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            board[cy + y][cx + x] = tetromino->block[y][x];
        }
    }
}
