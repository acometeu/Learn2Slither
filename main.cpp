#include "include/visuals.hpp"
#include "include/Agent.hpp"


int game_loop(Board &board, Snake &snake, MyArgs &args){

    snake.update_vision();
    snake.print_vision();

    Agent   agent(args.epsilon, args.alpha, args.gamma);
    // if (!args.import_path.empty())
    // {
    //     if (agent.set_import_path(args.import_path))
    //     {
    //         std::cerr << "import path invalid" << std::endl;
    //         return (1);
    //     }
    // }
    // if (!args.export_path.empty())
    // {
    //     if (agent.set_export_path(args.export_path))
    //     {
    //         std::cerr << "export path invalid" << std::endl;
    //         return (1);
    //     }
    // }

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

    std::cout << "export path = " << args.export_path << std::endl;
    return(0);
    // seed random number generator
    srand(time(NULL));
    Board board(args.board_size);
    Snake snake(board, args.snake_size);
    board.print_board();

    if (game_loop(board, snake, args))
        return (1);

    return 0;
}
