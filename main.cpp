#include "include/visuals.hpp"


int game_loop(Board &board, Snake &snake, MyArgs &args, Agent &agent){

    for (int i = 1; i <= args.sessions; i++)
    {
        bool    running = true;
        while(running)
        {
            std::cout << "session" << i << std::endl;
            if (snake.update_position_and_q_values(args, agent))
                break;
            snake.dir = agent.choose_direction(snake, args, i);
        }
        snake.stats_add_session();
        if (i < args.sessions - 1)
        {
            board.reset();
            snake.reset();
        }
    }

    snake.display_stats();
    if (!args.export_path.empty())
    {
        if (agent.save_q_table_to_export_path())
            return (1);
    }
    return(0);
}

int learn2slither(Board &board, Snake &snake, MyArgs &args){

    snake.update_vision();
    snake.print_vision();

    AStateStrategy *strat = new SimpleStateStrategy();
    Agent   agent(args.epsilon, args.alpha, args.gamma, args.sessions, strat);
    if (!args.import_path.empty())
    {
        if (agent.set_import_path(args.import_path))
            return (1);
    }

    // // testsuppr
    // for (auto it = agent.q_table.begin(); it != agent.q_table.end(); it++)
    // {
    //     auto key = (*it).first;
    //     auto values = (*it).second;
    //     std::cout << key[LEFT].green_apple << '.' << key[LEFT].red_apple << '.'
    //               << key[LEFT].snake_tail << '.' << key[LEFT].wall << ',';
    //     std::cout << key[RIGHT].green_apple << '.' << key[RIGHT].red_apple << '.'
    //               << key[RIGHT].snake_tail << '.' << key[RIGHT].wall << ',';
    //     std::cout << key[UP].green_apple << '.' << key[UP].red_apple << '.'
    //               << key[UP].snake_tail << '.' << key[UP].wall << ',';
    //     std::cout << key[DOWN].green_apple << '.' << key[DOWN].red_apple << '.'
    //               << key[DOWN].snake_tail << '.' << key[DOWN].wall << ':';
    //     if (values[LEFT])
    //         std::cout << LEFT << values[LEFT] << ',';
    //     if (values[RIGHT])
    //         std::cout << RIGHT << values[RIGHT] << ',';
    //     if (values[UP])
    //         std::cout << UP << values[UP] << ',';
    //     if (values[DOWN])
    //         std::cout << DOWN << values[DOWN];
    //     std::cout << std::endl;
    // }


    if (!args.export_path.empty())
    {
        if (agent.set_export_path(args.export_path))
            return (1);
    }

    snake.dir = agent.choose_direction(snake, args, 1);

    if (args.visual_mode)
    {
        if (game_loop_SDL(board, snake, args, agent))
            return (1);
    }
    else
    {
        if (game_loop(board, snake, args, agent))
            return(1);
    }

    return (0);
}

#include "include/SimpleStateStrategy.hpp"
int main(int argc, char* argv[]) {
    MyArgs args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    // seed random number generator
    Board board(args.board_size);
    Snake snake(board, args.snake_size);
    board.print_board();

    if (learn2slither(board, snake, args))
        return (1);

    return 0;
}
