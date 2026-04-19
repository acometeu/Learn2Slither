#include "include/visuals.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
    MyArgs args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    Board board(args.board_size);
    Snake snake(board, args.snake_size);
    board.print_board();
    snake.print_vision();


    if (args.visual)
    {
        if (run_SDL(board, snake, args))
            return (1);
    }

    return 0;
}
