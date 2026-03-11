#include "include/visuals.hpp"


int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    Board board(args.board_size);
    Snake snake(board, args.snake_size);
    board.print_board();

    if (args.visual)
    {
        if (run_SDL(board, snake))
            return (1);
    }

    return 0;
}
