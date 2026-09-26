#ifndef MENU_H
#define MENU_H

#include <SDL3/SDL.h>
#include "board.h"

/**
 * @brief Menu Properties.
 */
typedef enum {
    /**
     * @brief Menu is not visible.
     */
    MENU_INVISIBLE = 0,

    /**
     * @brief Menu is visible.
     */
    MENU_VISIBLE = 1,

    /**
     * @brief Menu is docked to the left of the screen.
     */
    MENU_DOCK_LEFT = 2,

    /**
     * @brief Menu is docked to the right of the screen.
     */
    MENU_DOCK_RIGHT = 4,

    /**
     * @brief Menu is docked to the center of the screen.
     */
    MENU_DOCK_CENTER = 8,

    /**
     * @brief Menu is docked to the entire screen.
     */
    MENU_FULL_SCREEN = 14
} MenuProperties;

typedef enum {
    BUTTON_NORMAL = 0,
    BUTTON_HIGHLIGHT = 1,
    BUTTON_DOWN = 2,
    BUTTON_DISABLE = 3
} ButtonState;

typedef struct {
    char *name;
    SDL_Point location;
    Size size;
    int font_size;
    void (*clicked)(void *arg);
    void (*double_clicked)(void *arg);
    SDL_Color normal;
    SDL_Color highlight;
    SDL_Color down;
    SDL_Color disabled;
    SDL_Color text_color;
    ButtonState button_state;
} Button;



void menu_init(void);
void menu_set_properties(MenuProperties properties);
void menu_show(void);
void menu_hide(void);
void menu_draw(void);
void menu_update(SDL_Event *event);

#endif