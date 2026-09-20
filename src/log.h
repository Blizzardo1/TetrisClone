#ifndef LOG_H
#define LOG_H

#include <SDL3/SDL.h>
#include <stdarg.h>

/**
 * @brief Custom Tetris Categories.
 */
typedef enum {
    /**
     * @brief The category for the core of the game.
     */
    CATEGORY_TETRIS = 1,
    /**
     * @brief The category for a Tetromino.
     */
    CATEGORY_PIECE = 2,
    /**
     * @brief The category for the Tetris board.
     */
    CATEGORY_BOARD = 3,
    /**
     * @brief The category for the Input.
     */
    CATEGORY_INPUT = 4,
    /**
     * @brief The category for the Timer.
     */
    CATEGORY_TIMER = 5
} TetrisCategory;

/**
 * @brief Log Modules
 */
typedef enum {
    /**
     * @brief The Time Module.
     */
    LOG_MODULE_TIME = 1,
    /**
     * @brief The Date Module.
     */
    LOG_MODULE_DATE = 2,
    /**
     * @brief The Category Module.
     */
    LOG_MODULE_CATEGORY = 4,
    /**
     * @brief The Priority Module.
     */
    LOG_MODULE_PRIORITY = 8
} LogModules;

/**
 * @brief Format of the log output
 * @remarks Accepted formats are based on LogModules:
 * $t : Time
 * $d : Date
 * $c : Category
 * $p : Priority
 */
typedef struct {
    LogModules modules;
    char *format;
} LogFormat;

/**
 * @brief Initializes the logger.
 */
void log_init(void);

/**
 * @brief Sets the current Category and Priority.
 *
 * @param category the new category. See Remarks for more information.
 * @param priority the new priority.
 * @remarks If the category is set to SDL_LOG_CATEGORY_CUSTOM,
 * then it must be appended with a TetrisCategory.
 * e.g SDL_LOG_CATEGORY_CUSTOM | CUSTOM_TETRIS
 */
void log_set(int sdl_category, SDL_LogPriority priority);

/**
 * @brief Gets the current format for the Logger.
 *
 * @return LogFormat the currently set log formatter.
 */
LogFormat log_get_formatter(void);

/**
 * @brief Sets the format for the Logger.
 *
 * @param format the new log formatter
 */
void log_set_formatter(LogFormat format);

#endif