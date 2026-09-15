#include <SDL3/SDL.h>
#include <SDL3/SDL_thread.h>

#include "tetris.h"

#define WINDOW_TITLE "Tetris"
#define INIT_SCREEN_WIDTH 1920
#define INIT_SCREEN_HEIGHT 1080

void draw() {
    tetris_draw();
}

void update(SDL_Event *event) {
    tetris_update(event);
}

int SDLCALL loop(void *data) {
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

    tetris_init(window, renderer, INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT);

    (void)data;
    SDL_Log("Game Loop started.");
    SDL_Event event;
    while(tetris_get_state()) {
        SDL_PollEvent(&event);
        if(event.type == SDL_EVENT_POLL_SENTINEL) {
            SDL_Delay(10);
        }
        draw();
        update(&event);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

int main(void) {

    // Create a thread that allows the user to remain in control of Window actions.
    SDL_Thread *thread = SDL_CreateThread(loop, "game_loop", (void *)NULL);
    int return_value;

    if(thread == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Game Loop thread. %s", SDL_GetError());
        return 1;
    }

    // Wait here until the game ends.
    SDL_WaitThread(thread, &return_value);
    SDL_Quit();
    return return_value;
}