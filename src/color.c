#include "color.h"
#include <math.h>

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

/**
 * @brief Color Int to Float.
 *
 * @param c convert base color from int.
 * @return float converted int to float.
 */
float citf(uint8_t c) {
    return (float)c / 255;
}

/**
 * @brief Color Float to Int.
 *
 * @param f convert base color from float.
 * @return uint8_t converted float to int.
 */
uint8_t cfti(float f) {
    return (uint8_t)f * 255;
}

typedef struct {
    float r_prime;
    float g_prime;
    float b_prime;
    float c_max;
    float c_min;
    float c_delta;
} ColorBaseLine;

ColorBaseLine color_base_line(SDL_Color color) {
    ColorBaseLine base = {
        .r_prime = citf(color.r),
        .g_prime = citf(color.g),
        .b_prime = citf(color.b)
    };

    base.c_max = color_max(base.r_prime, base.g_prime, base.b_prime);
    base.c_min = color_min(base.r_prime, base.g_prime, base.b_prime);
    base.c_delta = base.c_max - base.c_min;
    return base;
}

float color_get_hue(SDL_Color color) {
    ColorBaseLine base = color_base_line(color);
    return base.c_max == base.r_prime? (float)fmod(base.g_prime - base.b_prime / base.c_delta, 6) * 60 :
              base.c_max == base.g_prime ? (float)(base.b_prime - base.r_prime / base.c_delta) + 2 :
              base.c_max == base.b_prime ? (float)(base.r_prime - base.g_prime / base.c_delta) + 4 : 0;
}

float color_get_saturation(SDL_Color color) {
    ColorBaseLine base = color_base_line(color);
    return base.c_max > 0 ? base.c_delta / base.c_max : 0;
}

float color_get_value(SDL_Color color) {
    return color_base_line(color).c_max;
}

void color_set_hue(SDL_Color *color, float hue) {
    HSV hsv = color_rgb_to_hsv(*color);
    hsv.hue = hue;
    SDL_Color c = color_hsv_to_rgb(hsv);
    color = &c;
}

void color_set_saturation(SDL_Color *color, float saturation) {

}

void color_set_value(SDL_Color *color, float value) {

}

float color_max(float r, float g, float b) {
    return r > g ? r > b ? r : g > b ? g : b : 255;
}

float color_min(float r, float g, float b) {
    return r < g ? r < b ? r : g < b ? g : b : 0;
}

HSV color_rgb_to_hsv(SDL_Color color) {
    return (HSV) {
        .hue = color_get_hue(color),
        .saturation = color_get_saturation(color),
        .value = color_get_value(color)
    };
}

SDL_Color color_hsv_to_rgb(HSV hsv) {
    float C = hsv.value * hsv.saturation;
    float X = C * (1 - fabs(fmod(hsv.hue / 60, 2) - 1));
    float m = hsv.value - C;
    SDL_Color color = hsv.hue > 0 && hsv.hue < 60 ? (SDL_Color){C, X, 0, 255} :
                      hsv.hue > 60 && hsv.hue < 120 ? (SDL_Color){X, C, 0, 255} :
                      hsv.hue > 120 && hsv.hue < 180 ? (SDL_Color){0, C, X, 255} :
                      hsv.hue > 180 && hsv.hue < 240 ? (SDL_Color){0, X, C, 255} :
                      hsv.hue > 240 && hsv.hue < 300 ? (SDL_Color){X, 0, C, 255} :
                      hsv.hue > 300 && hsv.hue < 360 ? (SDL_Color){C, 0, X, 255} :
                      // Unbound Color Hue I suppose.
                      BLACK;

    return color;
}

void color_set_hsv(SDL_Color *color, HSV hsv) {
    *color = color_hsv_to_rgb(hsv);
}

SDL_Color color_determine_inverse(SDL_Color base) {
    // We want to process whether the output should be white or black,
    // depending on the lightness of the color passed in.
    // We want to be able to have readable text, and I know this function name,
    // needs some justice.

    HSV hsv = color_rgb_to_hsv(base);
    SDL_Log("Passed RGB(%d, %d, %d): HSV(%f, %f, %f)", base.r, base.g, base.b, hsv.hue, hsv.saturation, hsv.value);
    return hsv.value > .5 ? BLACK : WHITE;
}