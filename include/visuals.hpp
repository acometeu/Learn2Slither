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
#define STEP_RATE_MILISECOND 500
// #define SNAKE_SQUARE_PIXELS 60

// game colors
constexpr t_rgb BACKGROUND_COLOR {150, 150, 150, 255};
constexpr t_rgb GRID {200, 200, 200, 200};
constexpr t_rgb WALLS {50, 50, 50, 50};
constexpr t_rgb SNAKE_HEAD_COLOR {255, 255, 0, 0};
constexpr t_rgb SNAKE_BODY_COLOR {255, 0, 255, 0};
constexpr t_rgb GREEN_APPLE {0, 255, 0, 0};
constexpr t_rgb RED_APPLE {255, 0, 0, 0};

struct SDL_pixel
{
    float height;
    float width;
};

struct SDL_state
{
    // display settings
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    int width, height, logW, logH;

    // game time
    uint64_t    prev_time;

    // game objects
    SDL_pixel   pixel;
    std::vector< std::array<SDL_FPoint, 2> >    grid;
    std::vector<SDL_FRect>  walls;
    SDL_FRect   snake;
};


// Visual function declarations
int     run_SDL(Board &board, Snake &snake);
int     initialize(SDL_state &state);
void    initialize_objects(Board board, SDL_state &state);
std::vector<SDL_FRect>  initialize_walls(Board &board, SDL_state &state);
std::vector< std::array<SDL_FPoint, 2> >    initialize_grid(Board &board, SDL_state &state);
void    cleanup(SDL_state &state);
void    sdl_draw_snake(SDL_state &state, Snake &snake);
void    sdl_draw_snake_color(SDL_state &state, Snake &snake, t_rgb color, size_t pos);
void    sdl_draw_fruits(SDL_state &state, Board &board);


#endif