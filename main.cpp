#include "include/visuals.hpp"
#include "include/Agent.hpp"


int game_loop(Board &board, Snake &snake, MyArgs &args){

    snake.update_vision();
    snake.print_vision();

    Agent   agent;

    if (args.visual)
    {
        if (run_SDL(board, snake, args))
            return (1);
    }
    else
    {
        bool    running = true;
        while(running)
        {
            if (snake.update_position_and_vision())
                break;
        }
    }

    return (0);
}

int main(int argc, char* argv[]) {
    MyArgs args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    Board board(args.board_size);
    Snake snake(board, args.snake_size);
    board.print_board();

    if (game_loop(board, snake, args))
        return (1);

    return 0;
}
