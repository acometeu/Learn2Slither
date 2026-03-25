#ifndef VISUALS_HPP
#define VISUALS_HPP

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <array>
#include <vector>
#include "Snake.hpp"
#include "utils.hpp"


#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
// #define STEP_RATE_MILISECOND 500
// #define SNAKE_SQUARE_PIXELS 60

// game colors
constexpr t_rgb BACKGROUND_COLOR {150, 150, 150, 0};
constexpr t_rgb GRID_COLOR {200, 200, 200, 0};
constexpr t_rgb WALLS_COLOR {50, 50, 50, 0};
constexpr t_rgb SNAKE_HEAD_COLOR {255, 255, 0, 0};
constexpr t_rgb SNAKE_BODY_COLOR {255, 0, 255, 0};
constexpr t_rgb GREEN_APPLE_COLOR {0, 255, 0, 0};
constexpr t_rgb RED_APPLE_COLOR {255, 0, 0, 0};
constexpr t_rgb NAN_COLOR {0, 0, 255, 0};

struct sdl_pixel
{
    float height;
    float width;
};

struct sdl_state
{
    // display settings
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    int width, height, logW, logH;

    // game time
    uint64_t    prev_time;

    // game objects
    sdl_pixel   pixel;
    std::vector< std::array<SDL_FPoint, 2> >    grid;
    std::vector<SDL_FRect>  walls;
};


// Visual function declarations
int     run_SDL(Board &board, Snake &snake, MyArgs args);
int     initialize(sdl_state &state);
void    initialize_objects(Board board, sdl_state &state);
std::vector<SDL_FRect>  initialize_walls(Board &board, sdl_state &state);
std::vector< std::array<SDL_FPoint, 2> >    initialize_grid(Board &board, sdl_state &state);
void    make_draw_command(sdl_state &state, Board &board, Snake &snake);
void    cleanup(sdl_state &state);
void    sdl_draw_background(sdl_state &state);
void    sdl_draw_walls(sdl_state &state);
void    sdl_draw_board_objects(sdl_state &state, Board &board);
void    sdl_choose_box_color(sdl_state &state, Board &board, int y, int x);
void    sdl_draw_grid(sdl_state &state);


#endif