#include "menu.h"
#include "color.h"
#include "src/input_handler.h"
#include "tetris.h"
#include "stdlib.h"

MenuProperties _properties;

char *message = "Pause";

Button buttons[2] = {0};
Button btn_continue;
Button btn_quit;

#define BUTTONS_SIZE sizeof(buttons) / sizeof(buttons[0])

const char* get_menu_properties_name() {
    if(_properties == MENU_VISIBLE ) {
        return "Menu Floating";
    }

    if(_properties == (MENU_VISIBLE | MENU_DOCK_LEFT)) {
        return "Menu Docked Left";
    }

    if(_properties == (MENU_VISIBLE | MENU_DOCK_RIGHT)) {
        return "Menu Docked Right";
    }

    if(_properties == (MENU_VISIBLE | MENU_DOCK_CENTER)) {
        return "Menu Docked Center";
    }

    if(_properties == (MENU_VISIBLE | MENU_FULL_SCREEN)) {
        return "Menu Full Screen";
    }

    return "Menu Invisible";
}

SDL_FRect translate_button(Button *b) {
    return (SDL_FRect) {b->location.x, b->location.y,
        b->size.width, b->size.height};
}

void continue_clicked(void *arg) {
    (void)arg;
    handle_disable_menu();
    menu_hide();
}

void quit_clicked(void *arg) {
    (void)arg;
    tetris_set_state(false);
}

void draw_button(Button *b) {
    SDL_Color dc = BLACK;
    switch(b->button_state) {
        case BUTTON_NORMAL:
            dc = b->normal;
            break;
        case BUTTON_HIGHLIGHT:
            dc = b->highlight;
            break;
        case BUTTON_DOWN:
            dc = b->down;
            break;
        case BUTTON_DISABLE:
            dc = b->disabled;
            break;
    }

    SDL_Point p = b->location;
    Size sz = b->size;
    Size *text_sz = board_measure_string(b->name, b->font_size);
    board_fill_rect(p.x, p.y,
        sz.width, sz.height, dc);

    board_draw_text(b->name, ((b->location.x + b->size.width) / 2) - (text_sz->width / 2),
        (b->location.y + b->size.height) / 2 - (text_sz->height / 2),
         b->font_size, b->text_color);
}

void menu_init(void) {
    _properties = MENU_INVISIBLE | MENU_FULL_SCREEN;
    btn_continue = (Button) {
        .name = "Continue",
        .size = (Size) {
            .width = 150,
            .height = 36
        },
        .location = (SDL_Point) {
            .x = (tetris_get_window_width() / 2) - (150 / 2),
            .y = ((tetris_get_window_height() / 2) - (36 / 2)) + 40
        },
        .font_size = 20,
        .clicked = continue_clicked,
        .normal = LIGHTGRAY,
        .disabled = DARKGRAY,
        .highlight = YELLOW,
        .down = ORANGE,
        .text_color = WHITE,
        .button_state = BUTTON_NORMAL
    };

    btn_quit = (Button) {
        .name = "Quit",
        .size = (Size) {
            .width = 150,
            .height = 36
        },
        .location = (SDL_Point) {
            .x = (tetris_get_window_width() / 2) - (150 / 2),
            .y = ((tetris_get_window_height() / 2) - (36 / 2)) + (40 * 2)
        },
        .font_size = 20,
        .clicked = quit_clicked,
        .normal = LIGHTGRAY,
        .disabled = DARKGRAY,
        .highlight = YELLOW,
        .down = ORANGE,
        .text_color = WHITE,
        .button_state = BUTTON_NORMAL
    };
    buttons[0] = btn_continue;
    buttons[1] = btn_quit;
    for(size_t i = 0; i < BUTTONS_SIZE; i++) {
        Button b = buttons[i];
        SDL_Log("Button Registered \"%s\": (%d, %d); {%.2f, %.2f}",
        b.name, b.location.x, b.location.y, b.size.width, b.size.height);
    }
    SDL_Log("Window Pulled in Menu (%d, %d)", tetris_get_window_width(), tetris_get_window_height());
}

void menu_set_properties(MenuProperties properties) {
    _properties = properties;
}

void menu_show(void) {
    _properties |= MENU_VISIBLE;
    SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "[Menu set Visible] Menu Properties %d; %s", _properties, get_menu_properties_name());
}

void menu_hide(void) {
    _properties &= ~MENU_VISIBLE;
    SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "[Menu set Invisible] Menu Properties %d; %s", _properties, get_menu_properties_name());
}


/**
 * @brief Draws text on the Menu.
 * @remark This function will set the origin to the center of the text.
 *
 * @param text the text to render.
 * @param font_size the font size.
 * @param x the X-Position of where to put the text.
 * @param y the Y-Position of where to put the text.
 */
void menu_draw_text(char *text, size_t font_size, int x, int y) {
    Size *text_sz = board_measure_string(text, font_size);

    board_draw_text(message,
        x - ((float)text_sz->width / 2),
         y - ((float)text_sz->height / 2), 24, WHITE);

    free(text_sz);
}

void menu_draw_title() {
    menu_draw_text(message, 24, tetris_get_window_width() / 2, tetris_get_window_height() / 2);
}

void menu_draw(void) {
    SDL_Renderer *renderer = get_renderer();
    if(!(_properties & MENU_VISIBLE)) {
        return;
    }

    // Render a transparent black background and draw stuff
    SDL_Color color = DARKGRAY;
    color.a = 128;
    set_draw_color(color);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_FRect rect = {
        0,0,
        tetris_get_window_width(),
        tetris_get_window_height()};

    SDL_RenderFillRect(renderer, &rect);

    for(size_t i = 0; i < BUTTONS_SIZE; i++) {
        draw_button(&buttons[i]);
    }

    menu_draw_title();
}


bool intersects(SDL_FRect *a, SDL_FRect *b) {
    return a->x < b-> x + b->w &&
            a->x + a->w > b->x &&
            a->y < b->y + b->h &&
            a->y + a-> h > b-> y;
}

void menu_update(SDL_Event *event) {
    if(!(_properties & MENU_VISIBLE)) {
        return;
    }

    for(size_t i = 0; i < BUTTONS_SIZE; i++ ) {
        Button *b = &buttons[i];
        SDL_FRect br = translate_button(b);
        SDL_FRect mr = (SDL_FRect) {event->motion.x, event->motion.y, 1, 1};
        switch(event->type) {
            case SDL_EVENT_MOUSE_MOTION:
                if(intersects(&br, &mr)) {
                    b->button_state = BUTTON_HIGHLIGHT;
                } else {
                    b->button_state = BUTTON_NORMAL;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if(!intersects(&br, &mr)) {
                    continue;
                }

                if(event->button.clicks > 1 && b->double_clicked) {
                    b->double_clicked(b);
                } else if (b->clicked) {
                    b->clicked(b);
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                buttons[i].button_state = BUTTON_HIGHLIGHT;
                break;
            default:
                // pass everything else through
            break;
        }
    }
}