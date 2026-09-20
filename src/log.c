#include "log.h"
#include <stdio.h>
#include <time.h>

const char *token = "$[:word:]";
SDL_LogPriority _current_priority;
LogFormat _current_format;
int _current_category;
bool format_ready;


static void get_time_str(char *buf, size_t len) {
    time_t now = time(NULL);
    strftime(buf, len, "%H:%M:%S", localtime(&now));
}

static void get_date_str(char *buf, size_t len) {
    time_t now = time(NULL);
    strftime(buf, len, "%Y-%m-%d", localtime(&now));
}

static const char *custom_category(TetrisCategory category) {
    switch(category) {
        case CATEGORY_TETRIS:   return "TETRIS";
        case CATEGORY_PIECE:    return "PIECE";
        case CATEGORY_BOARD:    return "BOARD";
        case CATEGORY_INPUT:    return "INPUT";
        case CATEGORY_TIMER:    return "TIMER";
        default:                return "INVALID";
    }
}

static const char *category_name(SDL_LogCategory category) {
    switch (category) {
        case SDL_LOG_CATEGORY_APPLICATION:  return "APP";
        case SDL_LOG_CATEGORY_AUDIO:        return "AUD";
        case SDL_LOG_CATEGORY_ERROR:        return "ERR";
        case SDL_LOG_CATEGORY_SYSTEM:       return "SYS";
        case SDL_LOG_CATEGORY_ASSERT:       return "ASS";
        case SDL_LOG_CATEGORY_VIDEO:        return "VID";
        case SDL_LOG_CATEGORY_RENDER:       return "REN";
        case SDL_LOG_CATEGORY_INPUT:        return "INP";
        case SDL_LOG_CATEGORY_TEST:         return "TES";
        case SDL_LOG_CATEGORY_GPU:          return "GPU";

        /* Reserved for future SDL library use */
        case SDL_LOG_CATEGORY_RESERVED2:    return "RE2";
        case SDL_LOG_CATEGORY_RESERVED3:    return "RE3";
        case SDL_LOG_CATEGORY_RESERVED4:    return "RE4";
        case SDL_LOG_CATEGORY_RESERVED5:    return "RE5";
        case SDL_LOG_CATEGORY_RESERVED6:    return "RE6";
        case SDL_LOG_CATEGORY_RESERVED7:    return "RE7";
        case SDL_LOG_CATEGORY_RESERVED8:    return "RE8";
        case SDL_LOG_CATEGORY_RESERVED9:    return "RE9";
        case SDL_LOG_CATEGORY_RESERVED10:   return "REA";
        case SDL_LOG_CATEGORY_CUSTOM:       return custom_category(_current_category - SDL_LOG_CATEGORY_CUSTOM);
        default:                            return "INVALID";
    }
}

static const char *priority_name(SDL_LogPriority priority) {
    switch (priority) {
        case SDL_LOG_PRIORITY_TRACE:    return "TRACE";
        case SDL_LOG_PRIORITY_VERBOSE:  return "VERBOSE";
        case SDL_LOG_PRIORITY_DEBUG:    return "DEBUG";
        case SDL_LOG_PRIORITY_INFO:     return "INFO";
        case SDL_LOG_PRIORITY_WARN:     return "WARN";
        case SDL_LOG_PRIORITY_ERROR:    return "ERROR";
        case SDL_LOG_PRIORITY_CRITICAL: return "CRITICAL";
        case SDL_LOG_PRIORITY_INVALID:  return "INVALID";
        case SDL_LOG_PRIORITY_COUNT:    return "COUNT";
        default:                        return "UNKNOWN";
    }
}

int log_format_message(char *buffer, size_t buff_size, SDL_LogCategory category, SDL_LogPriority priority) {
    if(!format_ready) return -1;

    const char *cursor = _current_format.format;
    size_t pos = 0;

    while(*cursor != '\0' && pos + 1 < buff_size) {
        if(cursor[0] == '$' && cursor[1] != '\0') {
            char piece[64] = {0};
            int consumed = 1;
            switch(cursor[1]) {
                case 't':
                    if (_current_format.modules & LOG_MODULE_TIME)
                        get_time_str(piece, sizeof(piece));
                    break;
                case 'd':
                    if (_current_format.modules & LOG_MODULE_DATE)
                        get_date_str(piece, sizeof(piece));
                    break;
                case 'c':
                    if (_current_format.modules & LOG_MODULE_CATEGORY)
                        snprintf(piece, sizeof(piece), "%s", category_name(category));
                    break;
                case 'p':
                    if (_current_format.modules & LOG_MODULE_PRIORITY)
                        snprintf(piece, sizeof(piece), "%s", priority_name(priority));
                    break;
                default:
                    consumed = 0; /* This is not a recognized token,
                                    therefore treat '$' literally instead of parsing. */
                    break;
            }

            if(consumed) {
                size_t piece_len = strlen(piece);
                size_t space_left = buff_size - pos - 1;
                size_t copy_len = piece_len < space_left ? piece_len : space_left;
                memcpy(buffer + pos, piece, copy_len);
                pos += copy_len;
                cursor += 2; // We need to skip pass the current found token
                continue;
            }
        }
        buffer[pos++] = *cursor++;
    }
    buffer[pos] = '\0';
    return (int)pos;
}

void SDLCALL log_output(void *userdata, int category, SDL_LogPriority priority, const char *message) {
    (void)userdata;
    char format[512];
    if(log_format_message(format, sizeof(format), category, priority) == -1) {
        printf("Log Formatter is not ready.\n");
        return;
    }
    printf("%s %s\n", format, message);
}

void log_init() {
    SDL_SetLogOutputFunction(log_output, NULL);
    format_ready = true;
}

void log_set(int category, SDL_LogPriority priority) {
    _current_category = category;
    _current_priority = priority;
}

LogFormat log_get_formatter(void) {
    return _current_format;
}

void log_set_formatter(LogFormat format) {
    _current_format = format;
}