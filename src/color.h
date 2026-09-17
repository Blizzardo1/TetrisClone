#ifndef COLOR_H
#define COLOR_H

#include <SDL3/SDL.h>

#define LIGHTGRAY (SDL_Color){ 200, 200, 200, 255 }
#define BLACK (SDL_Color){0, 0, 0, 255}
#define WHITE (SDL_Color){255,255,255,255}

SDL_Renderer* get_renderer();
void color_init(SDL_Window *window, SDL_Renderer *renderer);
void set_draw_color(SDL_Color color);

#endif