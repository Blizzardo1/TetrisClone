#ifndef BOARD_H
#define BOARD_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define LOG_RENDER() SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Renderer is null or was never specified!");

void board_init(void);
void board_end(void);
void board_draw(SDL_Renderer *renderer, int x, int y, int width, int height);
void board_draw_plane(SDL_Renderer *renderer, SDL_Color color);
void board_draw_text(SDL_Renderer *renderer, const char *text, int x, int y, int fontSize, SDL_Color color);
void board_draw_rect(SDL_Renderer *renderer, int x, int y, int width, int height, SDL_Color color);
void board_draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color);
void board_fill_rect(SDL_Renderer *renderer, int x, int y, int width, int height, SDL_Color color);
void board_load_font(const char *fontPath, float fontSize);
void board_close_font(TTF_Font *font);

#endif