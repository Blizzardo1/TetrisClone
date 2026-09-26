#include "board.h"
#include "input_handler.h"
#include "menu.h"
#include "tetris.h"
#include "timer.h"
#include "color.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_thread.h>

int window_width;
int window_height;
SDL_BlendMode _mode;

bool tetris_running;

int tetris_get_window_width(void) {
    return window_width;
}

int tetris_get_window_height(void) {
    return window_height;
}

bool tetris_get_state(void) {
    return tetris_running;
}

void tetris_set_state(bool state) {
    tetris_running = state;
}

void tetris_lock_tetromino(char (*board)[BOARD_WIDTH], int board_width) {
    Tetromino *current = piece_get_current();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Locking Tetromino %s at XY(%d, %d)",
        current->name, current->x, current->y);

    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            if(current->block[row][col] != ' ') {
                board[current->y + row][current->x + col] = current->block[row][col];
            }
        }
    }
    piece_spawn(board_width);
}

void tetris_init(int ww, int wh) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing Tetris");
    window_width = ww;
    window_height = wh;
    timer_init();
    TTF_Init();
    board_init();
    update_viewport(ww, wh);
    piece_init(*get_board_width(), *get_board_height());
    board_clear();
    tetris_set_state(true);
}

void tetris_end(void) {
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

    // This will reset the board size and reset its location.
    board_set_size(win_w, win_h, TETROMINO_BLOCK_SIZE);
}

void tetris_draw(void) {
    SDL_Renderer *renderer = get_renderer();

    // Get the current Blend Mode. Some functions will change it.
    // So we want to get the default, basically,
    // then reset it at the end of the run.
    SDL_GetRenderDrawBlendMode(renderer, &_mode);

    if(!renderer) {
        LOG_RENDER();
        return;
    }

    Tetromino *current = piece_get_current();
    Tetromino *next = piece_get_next();
    // Defacto render clear. Doesn't matter what color really,
    // Black to me is easier on my eyes when debugging, but
    // You can do colors like CornflowerBlue for all I care.
    set_draw_color(BLACK);
    SDL_RenderClear(renderer);

    // Draw the background of the entire screen.
    // This can change based on what "Level" the player is at
    // when implemented.
    board_draw_plane(BLACK);

    // Draw a Border
    board_draw_rect(0,0,window_width - 1, window_height - 1, WHITE);

    // Draw the board by TETROMINO_BLOCK_SIZE, which by default is 32
    // This will calculate the 10x20 grid in 32x32 chunks.
    board_draw(TETROMINO_BLOCK_SIZE);

    // Some neat text.
    board_draw_text("Tetris", 10, 10, 16, WHITE);
    board_draw_text("Next", window_width - 200, 60, 16, WHITE);

    if(current) {
        piece_draw(current, current->x, current->y);
    }

    if(next) {
        // For some reason, the piece is way out to the right off screen.
        piece_draw(next, window_width - 300, 100);
    }

    if(handle_get_menu_state()) {
        menu_draw();
    }

    // Reset Blend mode to original to prevent any blend glitches.
    SDL_SetRenderDrawBlendMode(renderer, _mode);
    SDL_RenderPresent(renderer);
}

void thread_mouse_handle(SDL_Event *event) {
    Uint64 lag_ms = (SDL_GetTicksNS() - event->motion.timestamp) / 1000000;
    int pending = SDL_PeepEvents(NULL,
        0, SDL_PEEKEVENT,
         SDL_EVENT_MOUSE_MOTION, SDL_EVENT_MOUSE_MOTION);

    SDL_FPoint mouse_loc = {.x = event->motion.x, .y = event->motion.y};
    SDL_FPoint mouse_rel = {.x = event->motion.xrel, .y = event->motion.yrel};

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Mouse: Absolute (%f, %f); Relative (%f, %f); Lag: %llu ms, Pending: %d",
        mouse_loc.x, mouse_loc.y,
        mouse_rel.x, mouse_rel.y,
        (unsigned long long)lag_ms, pending
    );
}

void tetris_update(SDL_Event *event) {
    SDL_Thread *thread = SDL_CreateThread(timer_update, "MoveUpdate", (void *)NULL);
    Tetromino *piece = piece_get_current();
    if(!handle_get_menu_state())
        piece_update(piece, piece->x, piece->y);

    menu_update(event);

    switch(event->type) {
        case SDL_EVENT_KEY_DOWN:
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Key pressed: %s", SDL_GetKeyName(event->key.key));
            handle_input(&event->key);
            break;
        case SDL_EVENT_KEY_UP:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Key released: %s", SDL_GetKeyName(event->key.key));
            break;
        case SDL_EVENT_QUIT:
            SDL_Log("Quit event received");
            tetris_set_state(false);
            // Also force disable menu when Quit is received.
            handle_disable_menu();
            break;
        case SDL_EVENT_POLL_SENTINEL:
            // Still processing events, do not log
            break;
        case SDL_EVENT_MOUSE_MOTION:
            thread_mouse_handle(event);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Mouse button %d pressed at (%.2f, %.2f)",
                event->button.button, event->button.x, event->button.y);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Mouse button %d released at (%.2f, %.2f)",
                event->button.button, event->button.x, event->button.y);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Mouse wheel scrolled: (%.2f, %.2f)",
                event->wheel.x, event->wheel.y);
            break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Mouse left window");
            break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Mouse entered window");
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d resized to (%d, %d)",
                event->window.windowID, event->window.data1, event->window.data2);
            update_viewport(event->window.data1, event->window.data2);
            break;
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d close requested", event->window.windowID);
            tetris_set_state(false);
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d gained focus", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d lost focus", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MINIMIZED:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d minimized", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d maximized", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_RESTORED:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d restored", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MOVED:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d moved to (%d, %d)",
                event->window.windowID, event->window.data1, event->window.data2);
            break;
        case SDL_EVENT_WINDOW_EXPOSED:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d exposed", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_HIDDEN:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d hidden", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_SHOWN:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Window %d shown", event->window.windowID);
            break;
        default:
            // SDL_Log("Event type: %s - %d", sdl_event_name(event->type), event->type);
            break;
    }
    int result;
    SDL_WaitThread(thread, &result);
}