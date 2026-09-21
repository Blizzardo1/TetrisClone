#include <SDL3/SDL.h>
#include <SDL3/SDL_thread.h>

#include "color.h"
#include "log.h"
#include "tetris.h"

#define WINDOW_TITLE "Tetris"
#define INIT_SCREEN_WIDTH 1920
#define INIT_SCREEN_HEIGHT 1080

/**
 * @brief Draws to the screen.
 */
void draw() {
    tetris_draw();
}

/**
 * @brief Handles all updates.
 *
 * @param event a pointer to the polled SDL_Event.
 */
void update(SDL_Event *event) {
    tetris_update(event);
}

/**
 * @brief A threaded game loop
 *
 * @param data data to pass from the main thread.
 * @return int 0 on a clean and successful run; otherwise 1 for error/failure.
 */
int SDLCALL loop(void *data) {
    (void)data; // We're not using this.

    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Setup Logging
    log_init();
    LogFormat format = {
        .modules = LOG_MODULE_CATEGORY
                 | LOG_MODULE_PRIORITY
                 | LOG_MODULE_DATE
                 | LOG_MODULE_TIME,
                 .format = "[$d $t] ($c | $p)"
        };
    log_set_formatter(format);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

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

    color_init(window, renderer);
    // Initialize the game.
    tetris_init(window, renderer, INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Game Loop started.");

    SDL_Event event;
    while(tetris_get_state()) {
        SDL_PollEvent(&event);
        draw();
        update(&event);
        SDL_Delay(18);
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