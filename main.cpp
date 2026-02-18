#include <iostream>
#include "Board.hpp"

int main(void) {
    // std::cout << "Trop fort" << std::endl; 
    Board board(4);
    Snake snake(board);
    board.print_board();
    
    return (0);
}