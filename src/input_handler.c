#include "input_handler.h"


void handle_input(SDL_KeyboardEvent *event) {
    switch(event->key) {
        case HARD_DROP:
        tetris_move(piece_get_current(), 0, -1);
        break;
        case SOFT_DROP:
        tetris_move(piece_get_current(), 0, 1);
        break;
        case HOLD:
        break;
        case ROTATE:
        tetris_rotate(piece_get_current());
        break;
        case LEFT:
        tetris_move(piece_get_current(), -1, 0);
        break;
        case RIGHT:
        tetris_move(piece_get_current(), 1, 0);
        break;
        case MENU:
        tetris_end();
        break;
        default:
        // All other keys ignored
        break;
    }
}


