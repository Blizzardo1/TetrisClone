#include <stdlib.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "board.h"
#include "color.h"

TTF_Font *font = NULL;

int board_width;
int board_height;
SDL_Point board_loc;

char board[BOARD_HEIGHT][BOARD_WIDTH];

/**
 * @brief Internal function that is used to repeat characters.
 *
 * @param chr the character to repeat.
 * @param repeat number of times to repeat.
 * @return char* a string representing the string.
 */
char* repeat_str(const char chr, const int repeat) {
    // I don't know if I want to make this available across the program.
    // Unsure how useful it will be in other places. Created for checking against row clears.
    if(repeat < 0) {
        return NULL;
    }

    char *str = (char *)malloc((size_t)repeat + 1);

    if(!str) {
        return NULL;
    }

    memset(str, chr, (size_t)repeat);
    str[repeat] = '\0';

    return str;
}

char (*get_board())[BOARD_WIDTH] {
    return board;
}

void board_set_size(int win_w, int win_h, int tetromino_block_size) {
    // Should be 320x640
    board_width = BOARD_WIDTH * tetromino_block_size;
    board_height = BOARD_HEIGHT * tetromino_block_size;
    // Top-Left should be { .x = 800, .y = 220, .w = 320, .h = 640};
    board_loc.x = (win_w / 2) - (board_width / 2);
    board_loc.y = (win_h / 2) - (board_height / 2);
}

void board_clear() {
    // Easy enough call to blank out the entire board.
    memset(board, ' ', sizeof(board));
}

void board_clear_line(int row) {
    if(row < 0 || row >= BOARD_HEIGHT) {
        return;
    }

    for(int col = 0; col < BOARD_WIDTH; col++) {
        board[row][col] = ' ';
    }

    // Shift remaining blocks down if any
    for(int r = row; r > 0; --r) {
        memcpy(board[r], board[r - 1], sizeof(board[r]));
    }

    // Blanks out the top row.
    memset(board[0], ' ', sizeof(board[0]));
}

SDL_Point get_board_location(void) {
    return board_loc;
}

void set_board_location(SDL_Point loc) {
    board_loc = loc;
}

int* get_board_height(void) {
    return &board_height;
}

int* get_board_width(void) {
    return &board_width;
}

void board_init() {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing Board");
    board_load_font("JetBrainsMonoNerdFontPropo-Medium.ttf", 16);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
    }
    board_clear();
}

void board_end(void) {
    TTF_CloseFont(font);
}

void board_draw(int block_size) {
    SDL_Renderer *renderer = get_renderer();
    SDL_Color outer = {0xC0, 0xC0, 0xC0, 0xFF}; // 0xC0C0C0FF
    SDL_Color inner = {0xA0, 0xA0, 0xA0, 0xFF}; // 0xA0A0A0FF
    board_draw_rect(board_loc.x, board_loc.y, board_width, board_height, outer);
    board_draw_rect(board_loc.x + 1, board_loc.y + 1, board_width - 2, board_height - 2, inner);
    for (int r = 0; r < BOARD_HEIGHT; r++) {
        for (int c = 0; c < BOARD_WIDTH; c++) {
            if (board[r][c] == ' ') continue;
            SDL_FRect cell = {
                board_loc.x + c * block_size,
                board_loc.y + r * block_size,
                block_size,
                block_size
            };
            set_draw_color(WHITE);
            SDL_RenderFillRect(renderer, &cell);
        }
    }
}

void board_draw_plane(SDL_Color color) {
    SDL_Renderer *renderer = get_renderer();
    set_draw_color(color);
    SDL_RenderClear(renderer);
    // Additional drawing logic for the plane can be added here
    // TODO: Maybe add a texture or some other visual representation for the plane
}

void board_draw_text(const char *text, int x, int y, int fontSize, SDL_Color color) {
    if (!font) {
        SDL_Log("Invalid font");
        return;
    }

    int tmpsz = TTF_GetFontSize(font);
    TTF_SetFontSize(font, fontSize);

    SDL_Renderer *renderer = get_renderer();

    if(!renderer) {
        LOG_RENDER();
        return;
    }

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, strlen(text), color);
    if (!surface) {
        SDL_Log("Failed to create surface: %s", SDL_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }

    const SDL_FRect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderTexture(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    TTF_SetFontSize(font, tmpsz);
}

void board_draw_rect(int x, int y, int width, int height, SDL_Color color) {
    SDL_Renderer *renderer = get_renderer();
    if(!renderer) {
        LOG_RENDER();
        return;
    }
    set_draw_color(color);
    SDL_FRect frect = {x, y, width, height};
    SDL_RenderRect(renderer, &frect);
}

void board_fill_rect(int x, int y, int width, int height, SDL_Color color) {
    SDL_Renderer *renderer = get_renderer();
    if(!renderer) {
        LOG_RENDER();
        return;
    }

    set_draw_color(color);
    SDL_FRect frect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &frect);
}

void board_draw_line(int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_Renderer *renderer = get_renderer();
    if(!renderer) {
        LOG_RENDER();
        return;
    }

    set_draw_color(color);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
}

void board_load_font(const char *fontPath, float font_size) {
    font = TTF_OpenFont(fontPath, font_size);
    if(!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font %s", fontPath);
    }
}

Size* board_measure_string(char *text, int font_size) {
    Size* size = (Size*)malloc(sizeof(Size));
    int width, height;
    if(!TTF_GetStringSize(font, text, 0,
           &width, &height)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to measure string %s. %s", text, SDL_GetError());
            return 0;
    }
    size->width = width;
    size->height = height;

    return size;
}

void board_close_font(TTF_Font *font) {
    if (font) {
        TTF_CloseFont(font);
    }
}