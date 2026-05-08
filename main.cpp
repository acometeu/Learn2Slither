#include "include/visuals.hpp"
#include "include/Agent.hpp"


int game_loop(Board &board, Snake &snake, MyArgs &args){

    snake.update_vision();
    snake.print_vision();

    Agent   agent(args.epsilon, args.alpha, args.gamma);
    if (!args.import_path.empty())
    {
        if (agent.set_import_path(args.import_path))
            return (1);
    }
    // testsuppr display q_table
    // for (auto it = agent.q_table.begin(); it != agent.q_table.end(); it++)
    // {
    //     std::cout << (*it).first[LEFT] << "," << (*it).first[RIGHT] << "," << (*it).first[UP] << "," << (*it).first[DOWN] << std::endl;
    //     std::cout << "LEFT = " << (*it).second[LEFT] << std::endl;
    //     std::cout << "RIGHT = " << (*it).second[RIGHT] << std::endl;
    //     std::cout << "UP = " << (*it).second[UP] << std::endl;
    //     std::cout << "DOWN = " << (*it).second[DOWN] << std::endl;
    // }

    if (!args.export_path.empty())
    {
        if (agent.set_export_path(args.export_path))
            return (1);
    }

    if (args.visual_mode)
    {
        if (run_SDL(board, snake, args, agent))
            return (1);
    }
    else
    {
        bool    running = true;
        while(running)
        {
            if (snake.update_position_and_vision())
                break;
            // agent.choose_direction();
        }
    }

    return (0);
}

int main(int argc, char* argv[]) {
    MyArgs args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    //testtsuppr
    // std::cout << "std::stof(0.5) = " << std::stof("-0.5") << std::endl;
    // return(0);

    // seed random number generator
    srand(time(NULL));
    Board board(args.board_size);
    Snake snake(board, args.snake_size);
    board.print_board();

    if (game_loop(board, snake, args))
        return (1);

    return 0;
}
