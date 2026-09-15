#include <SDL3/SDL.h>

#include "tetris.h"

#define WINDOW_TITLE "Tetris"
#define INIT_SCREEN_WIDTH 1920
#define INIT_SCREEN_HEIGHT 1080

bool _running = false;

void draw() {
    tetris_draw();
}

void update(SDL_Event *event) {
    tetris_update(event);
}

int main(void) {

    SDL_Window * window = SDL_CreateWindow(WINDOW_TITLE, INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE );
    if (!window) {
        SDL_Log("Failed to spawn window: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if(!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }

    _running = true;
    tetris_init(window, renderer, INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT, &_running);

    SDL_Event event;
    while(_running) {
        SDL_PollEvent(&event);
        if(event.type != SDL_EVENT_POLL_SENTINEL) {
            SDL_Delay(10);
        }
        draw();
        update(&event);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}