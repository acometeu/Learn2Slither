#ifndef VISUALS_HPP
#define VISUALS_HPP

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <array>
#include <vector>
#include "Snake.hpp"
#include "utils.hpp"


#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 1200
#define STEP_RATE_MILISECOND 500
// #define SNAKE_SQUARE_PIXELS 60

// game colors
constexpr t_rgb BACKGROUND_COLOR {150, 150, 150, 255};
constexpr t_rgb BACKGROUND_LINES_COLOR {200, 200, 200, 200};
constexpr t_rgb SNAKE_HEAD_COLOR {255, 255, 0, 0};
constexpr t_rgb SNAKE_BODY_COLOR {0, 255, 0, 0};
constexpr t_rgb GREEN_APPLE {255, 0, 255, 0};
constexpr t_rgb RED_APPLE {255, 0, 0, 0};


struct SDL_state
{
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    int width, height, logW, logH;
    // int snake_width, snake_height;
};


// Visual function declarations
int run_SDL(Board &board, Snake &snake);

#endif