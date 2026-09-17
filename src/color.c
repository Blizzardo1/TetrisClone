#include "color.h"

SDL_Window *_window;
SDL_Renderer *_renderer;

void color_init(SDL_Window *window, SDL_Renderer *renderer) {
    _window = window;
    _renderer = renderer;
}

SDL_Renderer* get_renderer() {
    return _renderer;
}

void set_draw_color(SDL_Color c) {
    SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
}
