#include "board.h"
#include "input_handler.h"
#include "tetris.h"
#include "timer.h"

#define INIT_BOARD_WIDTH 10
#define INIT_BOARD_HEIGHT 20
#define SPAWN_CENTER_X 4
#define SPAWN_CENTER_Y 2
#define TETROMINO_BLOCK_SIZE 32

#define ASPECT_RATIO 16.f / 9.f // Respect 16:9

int window_width;
int window_height;
int board_width;
int board_height;

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

SDL_Color LIGHTGRAY = { 200, 200, 200, 255 };
SDL_Color BLACK = {0, 0, 0, 255};
SDL_Color WHITE = {255,255,255,255};

SDL_Window *_window = NULL;
SDL_Renderer *_renderer = NULL;
bool *running;
char board[INIT_BOARD_HEIGHT][INIT_BOARD_WIDTH];

Tetromino *current;
Tetromino *next;

Tetromino* tetromino_copy(Tetromino *tetromino) {
    Tetromino *t = malloc(sizeof(Tetromino));
    memcpy(t, tetromino, sizeof(Tetromino));
    return t;
}

Tetromino* get_random() {
    int rand = get_rand(0, 6);
    switch(rand) {
        case 0: return tetromino_copy(&J);
        case 1: return tetromino_copy(&L);
        case 2: return tetromino_copy(&I);
        case 3: return tetromino_copy(&O);
        case 4: return tetromino_copy(&S);
        case 5: return tetromino_copy(&T);
        case 6: return tetromino_copy(&Z);
    }
    // Should never be able to return a null Tetromino
    return NULL;
}

Tetromino* get_current() {
    return current;
}

void _set_draw_color(SDL_Color c) {
    SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
}

void _center_tetromino(void) {
    current->x = SPAWN_CENTER_X * ((board_width / 2) *  (TETROMINO_BLOCK_SIZE / 2));
    current->y = SPAWN_CENTER_Y * ((board_height  / 2) * (TETROMINO_BLOCK_SIZE / 2));
}

void tetris_clear_board() {
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            board[y][x] = ' ';
        }
    }
}

void tetris_lock_tetromino(void) {
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            if(current->block[row][col] != ' ') {
                board[current->y + row][current->x + col] = current->block[row][col];
            }
        }
    }
    current = next;
    _center_tetromino();
    next = get_random();
}

bool tetris_can_move(Tetromino *tetromino, int dx, int dy) {
    int nX = tetromino->x + dx;
    int nY = tetromino->y + dy;

    for(int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if(tetromino->block[row][col] == ' ') {
                continue;
            }
            int bx = nX + col;
            int by = nY + row;
            if (bx < 0 || bx > board_width || by >= board_height) {
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

void tetris_move(Tetromino *tetromino, int px, int py) {
    if(py == -1) {
        // Hard Drop
        while(tetris_can_move(tetromino, px, 1)) {
            tetromino->y++;
        }
    }
    if(!tetris_can_move(tetromino, px, py)) {
        return;
    }
    tetromino->x += px;
    tetromino->y += py;
}

void tetris_rotate(Tetromino *tetromino) {
    char temp[4][4];
    for(int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            temp[x][3 - y] = tetromino->block[y][x];
        }
    }
    memcpy(tetromino->block, temp, sizeof(tetromino->block));
}

void tetris_draw_tetromino(Tetromino *tetromino, int px, int py) {
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
            _set_draw_color(tetromino->color);
            SDL_RenderFillRect(_renderer, &rect);
            _set_draw_color(WHITE);
            SDL_RenderRect(_renderer, &rect);
        }
    }
}

void tetris_init(SDL_Window *window, SDL_Renderer *renderer, int ww, int wh, bool *run_state) {
    _window = window;
    _renderer = renderer;
    window_width = ww;
    window_height = wh;
    board_width = INIT_BOARD_WIDTH;
    board_height = INIT_BOARD_HEIGHT;
    running = run_state;
    TTF_Init();
    board_init();
    tetris_clear_board();
}

void tetris_end() {
    running = false;
    board_end();
}

void update_viewport(int win_w, int win_h) {
    const float target_aspect = ASPECT_RATIO;
    float win_aspect = (float)win_w / (float)win_h;

    SDL_Rect viewport;

    if(win_aspect > target_aspect) {
        viewport.h = win_h;
        viewport.w = (int)(win_h * target_aspect);
        viewport.x = (win_w - viewport.w) / 2;
        viewport.y = 0;
    } else {
        viewport.w = win_w;
        viewport.h = (int)(win_w / target_aspect);
        viewport.x = 0;
        viewport.y = (win_h - viewport.h) / 2;
    }

    SDL_SetRenderViewport(_renderer, &viewport);
    window_width = viewport.w;
    window_height = viewport.h;
    board_width = INIT_BOARD_WIDTH * TETROMINO_BLOCK_SIZE;
    board_height = INIT_BOARD_HEIGHT * TETROMINO_BLOCK_SIZE;
}

void tetris_draw(void) {
        _set_draw_color(BLACK);
        SDL_RenderClear(_renderer);
        board_draw_plane(_renderer, BLACK);
        board_draw(_renderer, (window_width / 2) - (board_width / 2), (window_height / 2) - (board_height / 2), board_width, board_height);
        board_draw_text(_renderer, "Tetris", 10, 10, 16, WHITE);
        board_draw_text(_renderer, "Next", 1800, 60, 16, LIGHTGRAY);
        if(current) {
            tetris_draw_tetromino(current, current->x, current->y);
        }
        if(next) {
            tetris_draw_tetromino(next, 1600, 100);
        }
        SDL_RenderPresent(_renderer);
}

void tetris_update(SDL_Event *event) {
    switch(event->type) {
        case SDL_EVENT_KEY_DOWN:
            SDL_Log("Key pressed: %s", SDL_GetKeyName(event->key.key));
            handle_input(&event->key);
            break;
        case SDL_EVENT_KEY_UP:
            SDL_Log("Key released: %s", SDL_GetKeyName(event->key.key));
            break;
        case SDL_EVENT_QUIT:
            SDL_Log("Quit event received");
            running = false;
            break;
        case SDL_EVENT_POLL_SENTINEL:
            // Still processing events, do not log
            break;
        case SDL_EVENT_MOUSE_MOTION:
            // SDL_Log("Mouse moved to (%.2f, %.2f)", event->motion.x, event->motion.y);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            SDL_Log("Mouse button %d pressed at (%.2f, %.2f)", event->button.button, event->button.x, event->button.y);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            SDL_Log("Mouse button %d released at (%.2f, %.2f)", event->button.button, event->button.x, event->button.y);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            SDL_Log("Mouse wheel scrolled: (%.2f, %.2f)", event->wheel.x, event->wheel.y);
            break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            SDL_Log("Mouse left window");
            break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            SDL_Log("Mouse entered window");
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_Log("Window resized to (%d, %d)", event->window.data1, event->window.data2);
            update_viewport(event->window.data1, event->window.data2);
            break;
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            SDL_Log("Window close requested");
            running = false;
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            SDL_Log("Window gained focus");
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            SDL_Log("Window lost focus");
            break;
        case SDL_EVENT_WINDOW_MINIMIZED:
            SDL_Log("Window minimized");
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED:
            SDL_Log("Window maximized");
            break;
        case SDL_EVENT_WINDOW_RESTORED:
            SDL_Log("Window restored");
            break;
        case SDL_EVENT_WINDOW_MOVED:
            SDL_Log("Window moved to (%d, %d)", event->window.data1, event->window.data2);
            break;
        case SDL_EVENT_WINDOW_EXPOSED:
            SDL_Log("Window exposed");
            break;
        case SDL_EVENT_WINDOW_HIDDEN:
            SDL_Log("Window hidden");
            break;
        case SDL_EVENT_WINDOW_SHOWN:
            SDL_Log("Window shown");
            break;
        default:
            // SDL_Log("Event type: %s - %d", sdl_event_name(event->type), event->type);
            break;
    }
    if(!current) {
        current = get_random();
        _center_tetromino();
    }
    if(!next) {
        next = get_random();
    }
}