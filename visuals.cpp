#include "include/visuals.hpp"

int game_loop_SDL(Board &board, Snake &snake, MyArgs &args, Agent &agent){
    
    sdl_state   state;

    if (initialize(state))
        return(1);
    initialize_objects(board, state);

    for (state.sessions = 1; state.sessions <= args.sessions; state.sessions++)
    {
        if (state.sessions > 1)
        {
            board.reset();
            snake.reset();
        }
        snake.dir = agent.choose_direction(snake, args, 1);
        std::cout << "session " << state.sessions << std::endl;
        while (state.running)
        {
            sdl_handle_event(snake, state);
            if (sdl_update_snake_position(snake, state, args, agent))
                break;
            // snake.dir = agent.choose_direction(snake);
            
            make_draw_command(state, board, snake);
            // swap buffers and present
            SDL_RenderPresent(state.renderer);
        }
        if (state.running == false)
            break;
        snake.stats_add_session();
    }
    cleanup(state);

    snake.display_stats();
    if (!args.export_path.empty())
    {
        if (agent.save_q_table_to_export_path())
            return (1);
    }
    return(0);
}

int     initialize(sdl_state &state){

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

    state.prev_time = SDL_GetTicks();
    state.event = { 0 };
    state.running = true;
    state.new_input = false;
    state.space_key = false;

    return(0);
}

void    initialize_objects(Board board, sdl_state &state){

    int wall_nbr = 2;
    state.pixel.height = state.pixel.width = ((WINDOW_WIDTH > WINDOW_HEIGHT) ? WINDOW_HEIGHT : WINDOW_WIDTH) / (board.get_board_size() + wall_nbr);
    state.grid = initialize_grid(board, state);
    state.walls = initialize_walls(board, state);
}

std::vector<SDL_FRect>    initialize_walls(Board &board, sdl_state &state){

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

std::vector< std::array<SDL_FPoint, 2> >    initialize_grid(Board &board, sdl_state &state){

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

int    sdl_update_snake_position(Snake &snake, sdl_state &state, MyArgs &args, Agent &agent){

    if (args.step_by_step_mode)
        return(sdl_update_snake_position_step_by_step_mode(snake, state, args, agent));

    return (sdl_update_snake_position_by_time(snake, state, args, agent));
}

int sdl_update_snake_position_step_by_step_mode(Snake &snake, sdl_state &state, MyArgs &args, Agent &agent){

    if (state.new_input)
    {
        if (state.space_key)
        {
            snake.dir = agent.choose_direction(snake, args, state.sessions);
            state.space_key = false;
        }

        state.new_input = false;
        if (snake.update_position_and_q_values(args, agent))
            return(1);
        // snake.dir = agent.choose_direction(snake, state.sessions);

        //testsuppr to get q_values
        auto keys = agent.strategy->encode(snake.get_snake_vision());
        auto values = agent.q_table[keys];
        std::cout << "values[LEFT] = " << values[LEFT] << std::endl;
        std::cout << "values[RIGHT] = " << values[RIGHT] << std::endl;
        std::cout << "values[UP] = " << values[UP] << std::endl;
        std::cout << "values[DOWN] = " << values[DOWN] << std::endl;
        // std::cout << "RIGHT:" << std::endl;
        // std::cout << "UP:" << std::endl;
        // std::cout << "DOWN:" << std::endl;


        // std::cout << "LEFT:" << std::endl;
        // std::cout << "keys[LEFT].green_apple = " << keys[LEFT].green_apple << std::endl;
        // std::cout << "keys[LEFT].red_apple = " << keys[LEFT].red_apple << std::endl;
        // std::cout << "keys[LEFT].snake_tail = " << keys[LEFT].obstacle << std::endl;
        // // std::cout << "keys[LEFT].wall = " << keys[LEFT].wall << std::endl;
        // std::cout << "RIGHT:" << std::endl;
        // std::cout << "keys[RIGHT].green_apple = " << keys[RIGHT].green_apple << std::endl;
        // std::cout << "keys[RIGHT].red_apple = " << keys[RIGHT].red_apple << std::endl;
        // std::cout << "keys[RIGHT].snake_tail = " << keys[RIGHT].obstacle << std::endl;
        // // std::cout << "keys[RIGHT].wall = " << keys[RIGHT].wall << std::endl;
        // std::cout << "UP:" << std::endl;
        // std::cout << "keys[UP].green_apple = " << keys[UP].green_apple << std::endl;
        // std::cout << "keys[UP].red_apple = " << keys[UP].red_apple << std::endl;
        // std::cout << "keys[UP].snake_tail = " << keys[UP].obstacle << std::endl;
        // // std::cout << "keys[UP].wall = " << keys[UP].wall << std::endl;
        // std::cout << "DOWN:" << std::endl;
        // std::cout << "keys[DOWN].green_apple = " << keys[DOWN].green_apple << std::endl;
        // std::cout << "keys[DOWN].red_apple = " << keys[DOWN].red_apple << std::endl;
        // std::cout << "keys[DOWN].snake_tail = " << keys[DOWN].obstacle << std::endl;
        // std::cout << "keys[DOWN].wall = " << keys[DOWN].wall << std::endl;


    }
    return(0);
}

int     sdl_update_snake_position_by_time(Snake &snake, sdl_state &state, MyArgs &args, Agent &agent){

    state.now_time = SDL_GetTicks();
    float   delta_time = state.now_time - state.prev_time;
    
    if (delta_time >= args.snake_speed || state.space_key == true)
    {
        state.space_key = false;
        if (snake.update_position_and_q_values(args, agent))
            return(1);
        snake.dir = agent.choose_direction(snake, args, state.sessions);
        state.prev_time = SDL_GetTicks();
    }
    return(0);
}

void sdl_handle_event(Snake &snake, sdl_state &state){

    while (SDL_PollEvent(&state.event))
    {
        switch (state.event.type)
        {
            case SDL_EVENT_QUIT:
            {
                state.running = false;
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:
            {
                state.width = state.event.window.data1;
                state.height = state.event.window.data2;
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                switch (state.event.key.scancode)
                {
                    case SDL_SCANCODE_ESCAPE:
                        state.running = false;
                        break;

                    // handle movements
                    case SDL_SCANCODE_D:
                        snake.dir = RIGHT;
                        state.new_input = true;
                        break;
                    case SDL_SCANCODE_A:
                        snake.dir = LEFT;
                        state.new_input = true;
                        break;
                    case SDL_SCANCODE_W:
                        snake.dir = UP;
                        state.new_input = true;
                        break;
                    case SDL_SCANCODE_S:
                        snake.dir = DOWN;
                        state.new_input = true;
                        break;

                    // special keys
                    case SDL_SCANCODE_SPACE:
                        // classic mode : max speed
                        // step-by-step mode : agent choose next direction
                        state.space_key = true;
                        state.new_input = true;

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
}

void    make_draw_command(sdl_state &state, Board &board, Snake &snake){

    sdl_draw_background(state);
    sdl_draw_walls(state);

    sdl_draw_board_objects(state, board);
    sdl_draw_grid(state);

        
}

void    sdl_draw_background(sdl_state &state){
    
    SDL_SetRenderDrawColor(state.renderer, BACKGROUND_COLOR.red, BACKGROUND_COLOR.green, BACKGROUND_COLOR.blue, BACKGROUND_COLOR.transparancy);
    SDL_RenderClear(state.renderer);
}


void    sdl_draw_walls(sdl_state &state){

    SDL_SetRenderDrawColor(state.renderer, WALLS_COLOR.red, WALLS_COLOR.green, WALLS_COLOR.blue, WALLS_COLOR.transparancy);
    for (int i = 0; i < state.walls.size(); i++)
        SDL_RenderFillRect(state.renderer, &state.walls[i]);
}

void    sdl_draw_board_objects(sdl_state &state, Board &board){

    SDL_FRect   box = {0, 0, state.pixel.width, state.pixel.height};
    int wall_size = state.pixel.width;
    for (int y = 0; y < board.map.size(); y++)
    {
        box.y = y * box.h + wall_size;
        for (int x = 0; x < board.map[y].size(); x++)
        {
            box.x = x * box.w + wall_size;
            sdl_choose_box_color(state, board, y, x);
            SDL_RenderFillRect(state.renderer, &box);
        }
    }
}

void    sdl_choose_box_color(sdl_state &state, Board &board, int y, int x){

    switch (board.map[y][x])
    {
    case EMPTY :
        SDL_SetRenderDrawColor(state.renderer, BACKGROUND_COLOR.red, BACKGROUND_COLOR.green, BACKGROUND_COLOR.blue, BACKGROUND_COLOR.transparancy);
        break;
    case HEAD :
        SDL_SetRenderDrawColor(state.renderer, SNAKE_HEAD_COLOR.red, SNAKE_HEAD_COLOR.green, SNAKE_HEAD_COLOR.blue, SNAKE_HEAD_COLOR.transparancy);
        break;
    case SNAKE :
        SDL_SetRenderDrawColor(state.renderer, SNAKE_BODY_COLOR.red, SNAKE_BODY_COLOR.green, SNAKE_BODY_COLOR.blue, SNAKE_BODY_COLOR.transparancy);
        break;
    case GREEN_APPLE :
        SDL_SetRenderDrawColor(state.renderer, GREEN_APPLE_COLOR.red, GREEN_APPLE_COLOR.green, GREEN_APPLE_COLOR.blue, GREEN_APPLE_COLOR.transparancy);
        break;
    case RED_APPLE :
        SDL_SetRenderDrawColor(state.renderer, RED_APPLE_COLOR.red, RED_APPLE_COLOR.green, RED_APPLE_COLOR.blue, RED_APPLE_COLOR.transparancy);
        break;
    default:
        std::cout << "CASE WITH UNKNOWN CASE" << std::endl;
        SDL_SetRenderDrawColor(state.renderer, NAN_COLOR.red, NAN_COLOR.green, NAN_COLOR.blue, NAN_COLOR.transparancy);
        break;
    }
}

void    sdl_draw_grid(sdl_state &state){

    SDL_SetRenderDrawColor(state.renderer, GRID_COLOR.red, GRID_COLOR.green, GRID_COLOR.blue, GRID_COLOR.transparancy);
    for (int i = 0; i < state.grid.size(); i++)
        SDL_RenderLine(state.renderer, state.grid[i][0].x, state.grid[i][0].y, state.grid[i][1].x, state.grid[i][1].y);
}

void    cleanup(sdl_state &state){
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}
