#ifndef BOARD_H
#define BOARD_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "color.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define LOG_RENDER() SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Renderer is null or was never specified!");


/**
 * @brief Gets the backend board.
 */
char (*get_board())[BOARD_WIDTH];

/**
 * @brief Get the board height.
 *
 * @return int* the board height.
 */
int* get_board_height();

/**
 * @brief Get the board width.
 *
 * @return int* the board width.
 */
int* get_board_width();

/**
 * @brief Sets the new size of the board.
 *
 * @param win_w the window width.
 * @param win_h the window height.
 * @param tetromino_block_size the block size of a single piece of a Tetromino.
 */
void board_set_size(int win_w, int win_h, int tetromino_block_size);

/**
 * @brief Initializes the Tetris Board.
 */
void board_init(void);

/**
 * @brief Frees any resources used by the board.
 */
void board_end(void);

/**
 * @brief Clears the Tetris board.
 * @param board the backend board.
 */
void board_clear(void);

/**
 * @brief Draws the board.
 *
 * @param x the X-Position.
 * @param y the Y-Position.
 * @param width the width of the board.
 * @param height the height of the board.
 */
void board_draw(int x, int y, int width, int height);

/**
 * @brief Draws a backplane to the screen.
 *
 * @param color the background color.
 */
void board_draw_plane(SDL_Color color);

/**
 * @brief Draws text to the screen.
 *
 * @param text the text to render.
 * @param x the X-Position of the text.
 * @param y the Y-Position of the text.
 * @param fontSize the size of the font to render.
 * @param color the text color.
 */
void board_draw_text(const char *text, int x, int y, int fontSize, SDL_Color color);

/**
 * @brief Draws a rectangle.
 *
 * @param x the X-Position.
 * @param y the Y-Position.
 * @param width the width.
 * @param height the height.
 * @param color the rectangle color
 */
void board_draw_rect(int x, int y, int width, int height, SDL_Color color);

/**
 * @brief Draws a line.
 *
 * @param x1 the first X-Position.
 * @param y1 the first Y-Position.
 * @param x2 the second X-Position.
 * @param y2 the second Y-Position.
 * @param color the color of the line.
 */
void board_draw_line(int x1, int y1, int x2, int y2, SDL_Color color);

/**
 * @brief Draws a filled rectangle.
 *
 * @param x the X-Position.
 * @param y the Y-Position.
 * @param width the width.
 * @param height the height.
 * @param color the recangle color.
 */
void board_fill_rect(int x, int y, int width, int height, SDL_Color color);

/**
 * @brief Loads a font for rendering.
 *
 * @param fontPath the path of the font to load.
 * @param fontSize the initial size of the font.
 */
void board_load_font(const char *fontPath, float fontSize);

/**
 * @brief Destroys any context of the font.
 *
 * @param font the font to stop using.
 */
void board_close_font(TTF_Font *font);

#endif