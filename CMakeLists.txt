cmake_minimum_required(VERSION 3.15)
project(Tetris LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define the main source files
set(SOURCES
    src/main.c
    src/tetris.c
    src/tetris.h
    src/piece.c
    src/piece.h
    src/board.c
    src/board.h
    src/input_handler.c
    src/input_handler.h
    src/timer.c
    src/timer.h
    src/color.c
    src/color.h
)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                                    /media/generalkenobi/Development/source/repos/SDL3/SDL/include
                                                    /media/generalkenobi/Development/source/repos/SDL3/SDL_image/include
                                                    /media/generalkenobi/Development/source/repos/SDL3/SDL_ttf/include)

list(APPEND CMAKE_PREFIX_PATH
    /run/media/generalkenobi/Development/source/repos/SDL3/SDL/build-linux
    /run/media/generalkenobi/Development/source/repos/SDL3/SDL_image/build-linux
    /run/media/generalkenobi/Development/source/repos/SDL3/SDL_ttf/build-linux
)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)


target_link_libraries(${PROJECT_NAME} PRIVATE
    pthread
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)
# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

configure_file(JetBrainsMonoNerdFontPropo-Medium.ttf . COPYONLY)

# Build type (Release/Debug)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()