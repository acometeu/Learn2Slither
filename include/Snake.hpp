#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <deque>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Board.hpp"
#include "utils.hpp"

class   Board; 


class Snake
{
public:
    Snake(Board &board, int &snake_size);
    ~Snake();

    // variables
    Board &board;


private:
    std::deque<t_coor>   _position;


    //private func
    int initialize_head(Board &board, int snake_size);
    int initialize_body(Board &board, int snake_size, int actual_size, t_coor &last_body_part);
    int choose_random_direction_initialisation(std::vector<int> &all_directions);
    int get_position_after_movement(Board &board, t_coor last_body, int direction, t_coor &body);


};



#endif