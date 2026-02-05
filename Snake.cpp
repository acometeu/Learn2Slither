#include "Snake.hpp"

Snake::Snake(Board  &board){
    // generate a pseudo random number generator
    srand(time(NULL));
    while (initialize_head(board))
    {
        std::cout << "Snake head 0 initialisation fail" << std::endl;
        continue;
    }
}

Snake::~Snake(){
    
}

int Snake::initialize_head(Board &board){
    t_coor  head;
    head.x = rand() % (board.get_board_size());
    head.y = rand() % (board.get_board_size());
    std::cout << "head.y = " << head.y << std::endl;
    std::cout << "head.x = " << head.x << std::endl;

    if (board.map[head.y][head.x] != '0')
        return(1);
    _position.push_back(head);
    board.map[head.y][head.x] = 'H';
    
    // if (initialize_body(board, 3, 2))
    // {
    //     _position.pop_back();
    //     return(1);
    // }
    return (0);
}

// int Snake::initialize_body(Board &board, int snake_size, int actual_size, t_coor last_body){

//     if (actual_size < snake_size)
//     {
//         std::vector<int>    all_directions = {UP, LEFT, RIGHT, DOWN};
//         direction = 

//     }
//     return(0)
// }

// t_coor  Snake::get_position_aftter_movement