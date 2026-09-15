#include "board.h"
#include <SDL3_ttf/SDL_ttf.h>

TTF_Font *font = NULL;

void board_init(void) {
    board_load_font("JetBrainsMonoNerdFontPropo-Medium.ttf", 16);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
    }
}

void board_end(void) {
    TTF_CloseFont(font);
}

void board_draw(SDL_Renderer *renderer, int x, int y, int width, int height) {
    SDL_Color outer = {0xC0, 0xC0, 0xC0, 0xFF}; // 0xC0C0C0FF
    SDL_Color inner = {0xA0, 0xA0, 0xA0, 0xFF}; // 0xA0A0A0FF
    board_draw_rect(renderer, x, y, width, height, outer);
    board_draw_rect(renderer, x + 1, y + 1, width - 2, height - 2, inner);
}

void board_draw_plane(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    // Additional drawing logic for the plane can be added here
    // TODO: Maybe add a texture or some other visual representation for the plane
}

void board_draw_text(SDL_Renderer *renderer, const char *text, int x, int y, int fontSize, SDL_Color color) {
    if (!font) {
        SDL_Log("Invalid font");
        return;
    }

    int tmpsz = TTF_GetFontSize(font);
    TTF_SetFontSize(font, fontSize);

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

void board_draw_rect(SDL_Renderer *renderer, int x, int y, int width, int height, SDL_Color color) {
    if (!renderer) {
        LOG_RENDER();
        return;
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect frect = {x, y, width, height};
    SDL_RenderRect(renderer, &frect);
}

void board_fill_rect(SDL_Renderer *renderer, int x, int y, int width, int height, SDL_Color color) {
    if(!renderer) {
        LOG_RENDER();
        return;
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect frect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &frect);
}

void board_draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color) {
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