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
    if (f < 0.0f) f = 0.0f;
    if (f > 1.0f) f = 1.0f;
    return (uint8_t)lroundf(f * 255.0f);
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
        ColorBaseLine b = color_base_line(color);
    if (b.c_delta == 0.0f) return 0.0f;           // achromatic

    float h;
    if (b.c_max == b.r_prime)
        h = fmodf((b.g_prime - b.b_prime) / b.c_delta, 6.0f);
    else if (b.c_max == b.g_prime)
        h = (b.b_prime - b.r_prime) / b.c_delta + 2.0f;
    else
        h = (b.r_prime - b.g_prime) / b.c_delta + 4.0f;

    h *= 60.0f;
    if (h < 0.0f) h += 360.0f;
    return h;
}

float color_get_saturation(SDL_Color color) {
    ColorBaseLine base = color_base_line(color);
    return base.c_max > 0 ? base.c_delta / base.c_max : 0;
}

float color_get_value(SDL_Color color) {
    return color_base_line(color).c_max;
}

void color_set_hue(SDL_Color *color, float hue) {
    if (!color) return;
    HSV hsv = color_rgb_to_hsv(*color);
    hsv.hue = hue;
    uint8_t a = color->a;
    *color = color_hsv_to_rgb(hsv);
    color->a = a;
}

void color_set_saturation(SDL_Color *color, float saturation) {
    if (!color) return;
    HSV hsv = color_rgb_to_hsv(*color);
    hsv.saturation = fminf(fmaxf(saturation, 0.0f), 1.0f);
    uint8_t a = color->a;
    *color = color_hsv_to_rgb(hsv);
    color->a = a;
}

void color_set_value(SDL_Color *color, float value) {
    if (!color) return;
    HSV hsv = color_rgb_to_hsv(*color);
    hsv.value = fminf(fmaxf(value, 0.0f), 1.0f);
    uint8_t a = color->a;
    *color = color_hsv_to_rgb(hsv);
    color->a = a;
}

float color_max(float r, float g, float b) {
    return fmaxf(r, fmaxf(g,b));
}

float color_min(float r, float g, float b) {
    return fminf(r, fminf(g, b));
}

HSV color_rgb_to_hsv(SDL_Color color) {
    return (HSV) {
        .hue = color_get_hue(color),
        .saturation = color_get_saturation(color),
        .value = color_get_value(color)
    };
}

SDL_Color color_hsv_to_rgb(HSV hsv) {
    float h = fmodf(hsv.hue, 360.0f);
    if (h < 0.0f) h += 360.0f;

    float C = hsv.value * hsv.saturation;
    float X = C * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = hsv.value - C;
    float r, g, b;

    switch ((int)(h / 60.0f)) {
        case 0:  r = C; g = X; b = 0; break;
        case 1:  r = X; g = C; b = 0; break;
        case 2:  r = 0; g = C; b = X; break;
        case 3:  r = 0; g = X; b = C; break;
        case 4:  r = X; g = 0; b = C; break;
        default: r = C; g = 0; b = X; break;   // 300..360
    }
    return (SDL_Color){ cfti(r + m), cfti(g + m), cfti(b + m), 255 };
}

void color_set_hsv(SDL_Color *color, HSV hsv) {
    *color = color_hsv_to_rgb(hsv);
}

SDL_Color color_contrast_text(SDL_Color base) {
    // YIQ perceived brightness, 0..255
    int y = (base.r * 299 + base.g * 587 + base.b * 114) / 1000;
    return y >= 128 ? BLACK : WHITE;
}