#include "include/visuals.hpp"

int run_SDL(Board &board, Snake &snake){
    
    SDL_state   state;
    
    if (initialize(state))
    return(1);
    
    
    // load game datas
    // const bool *keys = SDL_GetKeyboardState(nullptr);
    state.prev_time = SDL_GetTicks();
    // uint64_t    prev_time = SDL_GetTicks();
    
    initialize_objects(board, state);
    
    // start the game loop
    bool running = true;
    while (running)
    {
        uint64_t    now_time = SDL_GetTicks();
        float       delta_time = now_time - state.prev_time;
        
        // update snake pos
        if (delta_time >= STEP_RATE_MILISECOND)
        {
            // snake_step(snake, sdl_snake);
            snake.move(snake.dir);
            state.prev_time += STEP_RATE_MILISECOND;
        }
        
        SDL_Event   event = { 0 };
        while (SDL_PollEvent(&event))
        {
            // std::cout << "READING..." << std::endl;
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    switch (event.key.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                        running = false;
                        break;

                        // handle movements
                        case SDL_SCANCODE_D:
                        snake.dir = RIGHT;
                        break;
                        case SDL_SCANCODE_A:
                        snake.dir = LEFT;
                        break;
                        case SDL_SCANCODE_W:
                        snake.dir = UP;
                        break;
                        case SDL_SCANCODE_S:
                        snake.dir = DOWN;
                        break;
                        default :
                        break;
                    }
                    break;
                }
                default:
                {
                    // SDL_Log("Unhandled Event!");
                    break;
                }
            }
        }

        

        
        
        // draw background
        SDL_SetRenderDrawColor(state.renderer, BACKGROUND_COLOR.red, BACKGROUND_COLOR.green, BACKGROUND_COLOR.blue, BACKGROUND_COLOR.transparancy);
        SDL_RenderClear(state.renderer);
        
        // draw grid
        SDL_SetRenderDrawColor(state.renderer, GRID.red, GRID.green, GRID.blue, GRID.transparancy);
        for (int i = 0; i < state.grid.size(); i++)
        SDL_RenderLine(state.renderer, state.grid[i][0].x, state.grid[i][0].y, state.grid[i][1].x, state.grid[i][1].y);
        
        // draw walls
        SDL_SetRenderDrawColor(state.renderer, WALLS.red, WALLS.green, WALLS.blue, WALLS.transparancy);
        for (int i = 0; i < state.walls.size(); i++)
        SDL_RenderFillRect(state.renderer, &state.walls[i]);
        
        // sdl_draw_fruits
        sdl_draw_fruits(state, board);
        sdl_draw_snake(state, snake);
        
        
        // swap buffers and present
        SDL_RenderPresent(state.renderer);
    }
    cleanup(state);
    return(0);
}

int     initialize(SDL_state &state){

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return(1);
    }
    
    // create the window
    state.width = WINDOW_WIDTH;
    state.height = WINDOW_HEIGHT;
    // state.window = SDL_CreateWindow("Snake", state.width, state.height, SDL_WINDOW_RESIZABLE);
    state.window = SDL_CreateWindow("Snake", state.width, state.height, SDL_LOGICAL_PRESENTATION_DISABLED);
    if (!state.window)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        cleanup(state);
        return(1);
    }
    
    // create the renderer
    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer)
    {
        SDL_Log("Couldn't create renderer: %s", SDL_GetError());
        cleanup(state);
        return(1);
    }

    // configure presentation
    // state.logW = WINDOW_WIDTH;
    // state.logH = WINDOW_HEIGHT;
    // SDL_SetRenderLogicalPresentation(state.renderer, state.logW, state.logH, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return(0);
}

void    initialize_objects(Board board, SDL_state &state){

    int wall_nbr = 2;
    state.pixel.height = state.pixel.width = ((WINDOW_WIDTH > WINDOW_HEIGHT) ? WINDOW_HEIGHT : WINDOW_WIDTH) / (board.get_board_size() + wall_nbr);
    state.snake.h = state.pixel.height;
    state.snake.w = state.pixel.width;
    state.grid = initialize_grid(board, state);
    state.walls = initialize_walls(board, state);
}

std::vector<SDL_FRect>    initialize_walls(Board &board, SDL_state &state){

    std::vector<SDL_FRect>    walls;
    float wall_size = state.pixel.width;
    float pixel_limit = board.get_board_size() * state.pixel.width + (wall_size * 2);


    SDL_FRect   top_wall = {0, 0, pixel_limit, wall_size};
    walls.push_back(top_wall);

    SDL_FRect  bottom_wall = {0, pixel_limit - wall_size, pixel_limit, wall_size};
    walls.push_back(bottom_wall);

    SDL_FRect  left_wall = {0, wall_size, wall_size, pixel_limit - (wall_size * 2)};
    walls.push_back(left_wall);

    SDL_FRect  right_wall = {pixel_limit - wall_size, wall_size, wall_size, pixel_limit - (wall_size * 2)};
    walls.push_back(right_wall);

    return (walls);
}

std::vector< std::array<SDL_FPoint, 2> >    initialize_grid(Board &board, SDL_state &state){

    std::vector< std::array<SDL_FPoint, 2> >    grid;
    float wall_size = state.pixel.width;
    float pixel_limit = board.get_board_size() * state.pixel.width + wall_size;


    // initialize vertical lines
    for (float x = state.pixel.width; x <= pixel_limit; x += state.pixel.width)
    {
        SDL_FPoint   points[2] = {{x, wall_size}, {x, pixel_limit}};
        std::array<SDL_FPoint, 2>   line = {points[0], points[1]};
        grid.push_back(line);
    }
    
    // initialize horizontal lines
    for (float y = state.pixel.height; y <= pixel_limit; y += state.pixel.height)
    {
        SDL_FPoint   points[2] = {{wall_size, y}, {pixel_limit, y}};
        std::array<SDL_FPoint, 2>   line = {points[0], points[1]};
        grid.push_back(line);
    }

    return (grid);
}

void    cleanup(SDL_state &state){
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

void    sdl_draw_snake(SDL_state &state, Snake &snake){

    if (snake._position.size())
        sdl_draw_snake_color(state, snake, SNAKE_HEAD_COLOR, 0);
    for (size_t i = 1; i < snake._position.size(); i++)
        sdl_draw_snake_color(state, snake, SNAKE_BODY_COLOR, i);
}

void    sdl_draw_snake_color(SDL_state &state, Snake &snake, t_rgb color, size_t pos){
    t_coor  snake_pos;
    snake_pos = snake._position[pos];
    state.snake.x = snake_pos.x * state.snake.w + state.pixel.width;
    state.snake.y = snake_pos.y * state.snake.h + state.pixel.height;
    SDL_SetRenderDrawColor(state.renderer, color.red, color.green, color.blue, color.transparancy);
    SDL_RenderFillRect(state.renderer, &state.snake);
}

void    sdl_draw_fruits(SDL_state &state, Board &board){

    SDL_FRect  fruit_pos;
    fruit_pos.h = state.pixel.height;
    fruit_pos.w = state.pixel.width;
    SDL_SetRenderDrawColor(state.renderer, GREEN_APPLE.red, GREEN_APPLE.green, GREEN_APPLE.blue, GREEN_APPLE.transparancy);
    for (size_t i = 0; i < board.green_apple.size(); i++)
    {
        fruit_pos.x = board.green_apple[i].x * state.snake.w + state.pixel.width;
        fruit_pos.y = board.green_apple[i].y * state.snake.h + state.pixel.height;
        SDL_RenderFillRect(state.renderer, &fruit_pos);
    }
    SDL_SetRenderDrawColor(state.renderer, RED_APPLE.red, RED_APPLE.green, RED_APPLE.blue, RED_APPLE.transparancy);
    fruit_pos.x = board.red_apple.x * state.snake.w + state.pixel.width;
    fruit_pos.y = board.red_apple.y * state.snake.h + state.pixel.height;
    SDL_RenderFillRect(state.renderer, &fruit_pos);
}
