#ifndef BOARD_H
#define BOARD_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "color.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define LOG_RENDER() SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Renderer is null or was never specified!");


char (*get_board())[BOARD_WIDTH];
int* get_board_height();
int* get_board_width();
void board_set_size(int win_w, int win_h, int tetromino_block_size);

void board_init(void);
void board_end(void);
void board_clear(void);
void board_draw(int x, int y, int width, int height);
void board_draw_plane(SDL_Color color);
void board_draw_text(const char *text, int x, int y, int fontSize, SDL_Color color);
void board_draw_rect(int x, int y, int width, int height, SDL_Color color);
void board_draw_line(int x1, int y1, int x2, int y2, SDL_Color color);
void board_fill_rect(int x, int y, int width, int height, SDL_Color color);
void board_load_font(const char *fontPath, float fontSize);
void board_close_font(TTF_Font *font);

#endif