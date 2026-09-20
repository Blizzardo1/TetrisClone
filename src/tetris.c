#include "board.h"
#include "input_handler.h"
#include "tetris.h"
#include "timer.h"
#include "color.h"

int window_width;
int window_height;

bool tetris_running;


bool tetris_get_state() {
    return tetris_running;
}

void tetris_set_state(bool state) {
    tetris_running = state;
}

void tetris_lock_tetromino(char (*board)[BOARD_WIDTH], int board_width) {
    Tetromino *current = piece_get_current();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Locking Tetromino %s at XY(%d, %d)",
        current->name, current->x, current->y);

    for(int row = 0; row < BOARD_HEIGHT; row++) {
        for(int col = 0; col < BOARD_WIDTH; col++) {
            if(current->block[row][col] != ' ') {
                board[current->y + row][current->x + col] = current->block[row][col];
            }
        }
    }
    piece_spawn(board_width);
}

void tetris_init(SDL_Window *window, SDL_Renderer *renderer, int ww, int wh) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing Tetris");
    window_width = ww;
    window_height = wh;
    timer_init();
    TTF_Init();
    color_init(window, renderer);
    board_init();
    update_viewport(ww, wh);
    piece_init(*get_board_width(), *get_board_height());
    board_clear();
    tetris_set_state(true);
}

void tetris_end() {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Ending Game");
    tetris_set_state(false);
    board_end();
}

void update_viewport(int win_w, int win_h) {
    SDL_Renderer *renderer = get_renderer();
    if(!renderer) {
        LOG_RENDER();
        return;
    }

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

    SDL_SetRenderViewport(renderer, &viewport);
    window_width = viewport.w;
    window_height = viewport.h;
    board_set_size(win_w, win_h, TETROMINO_BLOCK_SIZE);
}

void tetris_draw(void) {
    SDL_Renderer *renderer = get_renderer();
    if(!renderer) {
        LOG_RENDER();
        return;
    }

    Tetromino *current = piece_get_current();
    Tetromino *next = piece_get_next();
    int *board_width = get_board_height();
    int *board_height = get_board_height();
    set_draw_color(BLACK);
    SDL_RenderClear(renderer);
    board_draw_plane(BLACK);
    board_draw((window_width / 2) - (*board_width / 2), (window_height / 2) - (*board_height / 2), *board_width, *board_height);
    board_draw_text("Tetris", 10, 10, 16, WHITE);
    board_draw_text("Next", 1800, 60, 16, LIGHTGRAY);
    if(current) {
        piece_draw(current, current->x, current->y);
    }
    if(next) {
        piece_draw(next, 1600, 100);
    }
    SDL_RenderPresent(renderer);
}


void tetris_update(SDL_Event *event) {
    SDL_Thread *thread = SDL_CreateThread(timer_update, "MoveUpdate", (void *)NULL);
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
            tetris_set_state(false);
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
            SDL_Log("Window %d resized to (%d, %d)", event->window.windowID, event->window.data1, event->window.data2);
            update_viewport(event->window.data1, event->window.data2);
            break;
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            SDL_Log("Window %d close requested", event->window.windowID);
            tetris_set_state(false);
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            SDL_Log("Window %d gained focus", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            SDL_Log("Window %d lost focus", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MINIMIZED:
            SDL_Log("Window %d minimized", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED:
            SDL_Log("Window %d maximized", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_RESTORED:
            SDL_Log("Window %d restored", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MOVED:
            SDL_Log("Window %d moved to (%d, %d)", event->window.windowID, event->window.data1, event->window.data2);
            break;
        case SDL_EVENT_WINDOW_EXPOSED:
            SDL_Log("Window %d exposed", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_HIDDEN:
            SDL_Log("Window %d hidden", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_SHOWN:
            SDL_Log("Window %d shown", event->window.windowID);
            break;
        default:
            // SDL_Log("Event type: %s - %d", sdl_event_name(event->type), event->type);
            break;
    }
    int result;
    SDL_WaitThread(thread, &result);
}