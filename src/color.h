#ifndef COLOR_H
#define COLOR_H

#include <SDL3/SDL.h>

// List of Colors
#define COLOR_LIST \
    X(BEIGE,          245, 245, 220, 255) \
    X(BLACK,          0,   0,   0,   255) \
    X(BLACK_ALPHA_50,  0,  0,   0,   128) \
    X(BLUE,           0,   0,   255, 255) \
    X(BROWN,          139, 69,  19,  255) \
    X(CORAL,          255, 127, 80,  255) \
    X(CORNFLOWERBLUE, 100, 149, 237, 255) \
    X(CRIMSON,        220, 20,  60,  255) \
    X(CYAN,           0,   255, 255, 255) \
    X(DARKGRAY,       128, 128, 128, 255) \
    X(GOLD,           255, 215, 0,   255) \
    X(GRAY,           169, 169, 169, 255) \
    X(GREEN,          0,   255, 0,   255) \
    X(INDIGO,         75,  0,   130, 255) \
    X(IVORY,          255, 255, 240, 255) \
    X(KHAKI,          240, 230, 140, 255) \
    X(LIGHTGRAY,      200, 200, 200, 255) \
    X(LIME,           50,  205, 50,  255) \
    X(MAGENTA,        255, 0,   255, 255) \
    X(MAROON,         128, 0,   0,   255) \
    X(NAVY,           0,   0,   128, 255) \
    X(OLIVE,          128, 128, 0,   255) \
    X(ORANGE,         255, 165, 0,   255) \
    X(PINK,           255, 192, 203, 255) \
    X(PURPLE,         128, 0,   128, 255) \
    X(RED,            255, 0,   0,   255) \
    X(SALMON,         250, 128, 114, 255) \
    X(SILVER,         192, 192, 192, 255) \
    X(TEAL,           0,   128, 128, 255) \
    X(TRANSPARENT,      0,   0,   0,   0) \
    X(TURQUOISE,      64,  224, 208, 255) \
    X(VIOLET,         238, 130, 238, 255) \
    X(WHITE,          255, 255, 255, 255) \
    X(WHITE_ALPHA_50,  255,255, 255, 128) \
    X(YELLOW,         255, 255, 0,   255)

typedef struct {
    const char *name;
    SDL_Color color;
} KnownColor;

typedef enum {
    #define X(name, r, g, b, a) COLOR_IDX_##name,
        COLOR_LIST
    #undef X
        COLOR_IDX_COUNT
} ColorIndex;

static const KnownColor COLOR_TABLE[] = {
    #define X(name, r, g, b, a) { #name, {r, g, b, a}},
        COLOR_LIST
    #undef X
};

typedef struct {
    float hue;
    float saturation;
    float value;
} HSV;

#define COLOR_TABLE_COUNT (sizeof(COLOR_TABLE) / sizeof(COLOR_TABLE[0]))

static const inline SDL_Color* color_lookup(const char *name) {
    for(size_t i = 0; i < COLOR_TABLE_COUNT; i++) {
        if(strcmp(COLOR_TABLE[i].name, name) == 0) {
            return &COLOR_TABLE[i].color;
        }
    }
    return NULL;
}

// Color Table; What's shown here is due to the above macro.
// If you update that macro, be sure to update this list as well.
#define BEIGE          (COLOR_TABLE[COLOR_IDX_BEIGE].color)
#define BLACK          (COLOR_TABLE[COLOR_IDX_BLACK].color)
#define BLACK_ALPHA_50 (COLOR_TABLE[COLOR_IDX_BLACK_ALPHA_50].color)
#define BLUE           (COLOR_TABLE[COLOR_IDX_BLUE].color)
#define BROWN          (COLOR_TABLE[COLOR_IDX_BROWN].color)
#define CORAL          (COLOR_TABLE[COLOR_IDX_CORAL].color)
#define CORNFLOWERBLUE (COLOR_TABLE[COLOR_IDX_CORNFLOWERBLUE].color)
#define CRIMSON        (COLOR_TABLE[COLOR_IDX_CRIMSON].color)
#define CYAN           (COLOR_TABLE[COLOR_IDX_CYAN].color)
#define DARKGRAY       (COLOR_TABLE[COLOR_IDX_DARKGRAY].color)
#define GOLD           (COLOR_TABLE[COLOR_IDX_GOLD].color)
#define GRAY           (COLOR_TABLE[COLOR_IDX_GRAY].color)
#define GREEN          (COLOR_TABLE[COLOR_IDX_GREEN].color)
#define INDIGO         (COLOR_TABLE[COLOR_IDX_INDIGO].color)
#define IVORY          (COLOR_TABLE[COLOR_IDX_IVORY].color)
#define KHAKI          (COLOR_TABLE[COLOR_IDX_KHAKI].color)
#define LIGHTGRAY      (COLOR_TABLE[COLOR_IDX_LIGHTGRAY].color)
#define LIME           (COLOR_TABLE[COLOR_IDX_LIME].color)
#define MAGENTA        (COLOR_TABLE[COLOR_IDX_MAGENTA].color)
#define MAROON         (COLOR_TABLE[COLOR_IDX_MAROON].color)
#define NAVY           (COLOR_TABLE[COLOR_IDX_NAVY].color)
#define OLIVE          (COLOR_TABLE[COLOR_IDX_OLIVE].color)
#define ORANGE         (COLOR_TABLE[COLOR_IDX_ORANGE].color)
#define PINK           (COLOR_TABLE[COLOR_IDX_PINK].color)
#define PURPLE         (COLOR_TABLE[COLOR_IDX_PURPLE].color)
#define RED            (COLOR_TABLE[COLOR_IDX_RED].color)
#define SALMON         (COLOR_TABLE[COLOR_IDX_SALMON].color)
#define SILVER         (COLOR_TABLE[COLOR_IDX_SILVER].color)
#define TEAL           (COLOR_TABLE[COLOR_IDX_TEAL].color)
#define TRANSPARENT    (COLOR_TABLE[COLOR_IDX_TRANSPARENT].color)
#define TURQUOISE      (COLOR_TABLE[COLOR_IDX_TURQUOISE].color)
#define VIOLET         (COLOR_TABLE[COLOR_IDX_VIOLET].color)
#define WHITE          (COLOR_TABLE[COLOR_IDX_WHITE].color)
#define WHITE_ALPHA_50 (COLOR_TABLE[COLOR_IDX_WHITE_ALPHA_50].color)
#define YELLOW         (COLOR_TABLE[COLOR_IDX_YELLOW].color)

/**
 * @brief Get the main renderer.
 *
 * @return SDL_Renderer* a pointer to the engine's main renderer.
 */
SDL_Renderer* get_renderer();
/**
 * @brief Initializes the backend renderer and window management.
 * @remarks Yes, I know... The Color system is handling passing the renderer and window throughout the entire code. I should find a better solution to this.
 *
 * @param window the window to pass around.
 * @param renderer the renderer to pass around.
 */
void color_init(SDL_Window *window, SDL_Renderer *renderer);

/**
 * @brief Set the current rendering draw color.
 *
 * @param color the color to set the global draw color to.
 */
void set_draw_color(SDL_Color color);

/**
 * @brief Determine whether the output color should be white or black depending on the color passed in.
 *
 * @param base the color to determine whether the output should be white or black.
 * @return SDL_Color the color white or black based on the value of the color.
 */
SDL_Color color_determine_inverse(SDL_Color base);

HSV color_rgb_to_hsv(SDL_Color color);
SDL_Color color_hsv_to_rgb(HSV hsv);

float color_get_hue(SDL_Color color);
float color_get_saturation(SDL_Color color);
float color_get_value(SDL_Color color);

void color_set_hue(SDL_Color *color, float hue);
void color_set_saturation(SDL_Color *color, float saturation);
void color_set_value(SDL_Color *color, float value);

void color_set_hsv(SDL_Color *color, HSV hsv);

float color_max(float r, float g, float b);
float color_min(float r, float g, float b);

#endif