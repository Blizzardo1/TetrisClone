#include "board.h"
#include <SDL3_ttf/SDL_ttf.h>

TTF_Font *font = NULL;

int board_width;
int board_height;
SDL_Point board_loc;

char board[BOARD_HEIGHT][BOARD_WIDTH];

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
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            board[y][x] = ' ';
        }
    }
}

SDL_Point get_board_location() {
    return board_loc;
}

void set_board_location(SDL_Point loc) {
    board_loc = loc;
}

int* get_board_height() {
    return &board_height;
}

int* get_board_width() {
    return &board_width;
}

void board_init() {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing Board");
    board_load_font("JetBrainsMonoNerdFontPropo-Medium.ttf", 16);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
    }
}

void board_end(void) {
    TTF_CloseFont(font);
}

void board_draw(int x, int y, int width, int height) {
    SDL_Color outer = {0xC0, 0xC0, 0xC0, 0xFF}; // 0xC0C0C0FF
    SDL_Color inner = {0xA0, 0xA0, 0xA0, 0xFF}; // 0xA0A0A0FF
    board_draw_rect(x, y, width, height, outer);
    board_draw_rect(x + 1, y + 1, width - 2, height - 2, inner);
}

void board_draw_plane(SDL_Color color) {
    SDL_Renderer *renderer = get_renderer();
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
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
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect frect = {x, y, width, height};
    SDL_RenderRect(renderer, &frect);
}

void board_fill_rect(int x, int y, int width, int height, SDL_Color color) {
    SDL_Renderer *renderer = get_renderer();
    if(!renderer) {
        LOG_RENDER();
        return;
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect frect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &frect);
}

void board_draw_line(int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_Renderer *renderer = get_renderer();
    if(!renderer) {
        LOG_RENDER();
        return;
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
}

void board_load_font(const char *fontPath, float fontSize) {
    font = TTF_OpenFont(fontPath, fontSize);
    if(!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font %s", fontPath);
    }
}

void board_close_font(TTF_Font *font) {
    if (font) {
        TTF_CloseFont(font);
    }
}