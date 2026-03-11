#include "include/visuals.hpp"

void    cleanup(SDL_state &state){
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

std::vector< std::array<SDL_FPoint, 2> >    initialize_board_line(Board &board, SDL_FRect &sdl_snake){

    std::vector< std::array<SDL_FPoint, 2> >    board_lines;
    float pixel_limit = board.get_board_size() * sdl_snake.w;


    // initialize vertical lines
    for (float x = sdl_snake.w; x <= pixel_limit; x += sdl_snake.w)
    {
        SDL_FPoint   points[2] = {{x, 0}, {x, pixel_limit}};
        std::array<SDL_FPoint, 2>   line = {points[0], points[1]};
        board_lines.push_back(line);
    }
    
    // initialize horizontal lines
    for (float y = sdl_snake.h; y <= pixel_limit; y += sdl_snake.h)
    {
        SDL_FPoint   points[2] = {{0, y}, {pixel_limit, y}};
        std::array<SDL_FPoint, 2>   line = {points[0], points[1]};
        board_lines.push_back(line);
    }

    return (board_lines);
}

int    initialize(SDL_state &state){

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

void    snake_step(Snake &snake, SDL_FRect &sdl_snake){

    switch (snake.dir)
    {
    case RIGHT:
        sdl_snake.x += sdl_snake.w;
        break;
    case LEFT:
        sdl_snake.x -= sdl_snake.w;
        break;
    case DOWN:
        sdl_snake.y += sdl_snake.h;
        break;
    case UP:
        sdl_snake.y -= sdl_snake.h;
        break;
    }
}

int run_SDL(Board &board, Snake &snake){

    SDL_state   state;

    if (initialize(state))
        return(1);


    // load game datas
    // const bool *keys = SDL_GetKeyboardState(nullptr);
    uint64_t    prev_time = SDL_GetTicks();

    // declare objects
    SDL_FRect sdl_snake;
    t_coor  snake_pos = snake.get_head_position();
    sdl_snake.w = sdl_snake.h = ((WINDOW_WIDTH > WINDOW_HEIGHT) ? WINDOW_HEIGHT : WINDOW_WIDTH) / board.get_board_size();
    sdl_snake.x = snake_pos.x * sdl_snake.w;
    sdl_snake.y = snake_pos.y * sdl_snake.h;
    int oui = 8;
    std::vector< std::array<SDL_FPoint, 2> >  board_lines = initialize_board_line(board, sdl_snake);

    // start the game loop
    bool running = true;
    while (running)
    {
        uint64_t    now_time = SDL_GetTicks();
        float       delta_time = now_time - prev_time;

        // update snake pos
        if (delta_time >= STEP_RATE_MILISECOND)
        {
            snake_step(snake, sdl_snake);
            prev_time += STEP_RATE_MILISECOND;
        }

        SDL_Event   event = { 0 };
        while (SDL_PollEvent(&event))
        {
            std::cout << "READING..." << std::endl;
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
                    }
                    break;
                }
                default:
                {
                    SDL_Log("Unhandled Event!");
                    break;
                }
            }
        }

        



        // perform drawing commands
        SDL_SetRenderDrawColor(state.renderer, 150, 150, 150, 255);
        SDL_RenderClear(state.renderer);
        
        // draw board
        SDL_SetRenderDrawColor(state.renderer, 200, 200, 200, 200);
        for (int i = 0; i < board_lines.size() - 1; i++)
            SDL_RenderLine(state.renderer, board_lines[i][0].x, board_lines[i][0].y, board_lines[i][1].x, board_lines[i][1].y);

        // draw head
        SDL_SetRenderDrawColor(state.renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(state.renderer, &sdl_snake);
        
        // swap buffers and present
        SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255);
        SDL_RenderPresent(state.renderer);
    }
    cleanup(state);
    return(0);
}