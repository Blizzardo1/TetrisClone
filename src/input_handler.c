#include "input_handler.h"
#include "menu.h"
#include "tetris.h"

bool _menu_visible;

void handle_hard_drop(void) {
    piece_move(piece_get_current(), 0, -1, get_board(), *get_board_width(), *get_board_height());
}

void handle_soft_drop(void) {
    piece_move(piece_get_current(), 0, 1, get_board(), *get_board_width(), *get_board_height());
}

void handle_hold(void) {

}

void handle_rotate(void) {
    piece_rotate(piece_get_current());
}

void handle_left(void) {
    piece_move(piece_get_current(), -1, 0, get_board(), *get_board_width(), *get_board_height());
}

void handle_right(void) {
    piece_move(piece_get_current(), 1, 0, get_board(), *get_board_width(), *get_board_height());
}

void handle_disable_menu(void) {
    _menu_visible = false;
}

bool handle_get_menu_state(void) {
    return _menu_visible;
}

void handle_menu(void) {
    _menu_visible = !_menu_visible;
    if(_menu_visible) {
        menu_show();
    } else {
        menu_hide();
    }
}


int SDLCALL input_thread(void *arg) {
    SDL_KeyboardEvent *event = (SDL_KeyboardEvent *)arg;
    switch(event->key) {
        case HARD_DROP:
        if(!handle_get_menu_state())
            handle_hard_drop();
        break;
        case SOFT_DROP:
        if(!handle_get_menu_state())
            handle_soft_drop();
        break;
        case HOLD:
        if(!handle_get_menu_state())
            handle_hold();
        break;
        case ROTATE:
        if(!handle_get_menu_state())
            handle_rotate();
        break;
        case LEFT:
        if(!handle_get_menu_state())
            handle_left();
        break;
        case RIGHT:
        if(!handle_get_menu_state())
            handle_right();
        break;
        case MENU:
        handle_menu();
        break;
        default:
        // All other keys ignored
        break;
    }
    return 0;
}


void handle_input(SDL_KeyboardEvent *event) {
    SDL_Thread *thread = SDL_CreateThread(input_thread, "InputThread", (void *)event);
    if(thread == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to handle input at this time: %s", SDL_GetError());
        return;
    }
    SDL_WaitThread(thread, NULL);
}


