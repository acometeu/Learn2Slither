#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "utils.hpp"
#include "Board.hpp"

class   Board; 


class Snake
{
public:
    Snake(Board &board, int &snake_size);
    ~Snake();

    // variables
    Board   &board;
    std::deque<t_coor>   _position;
    int     dir;

    // functions
    t_coor  get_head_position(void);
    int move(int direction);


private:


    //private func
    int initialize_head(Board &board, int snake_size);
    int initialize_body(Board &board, int snake_size, int actual_size, t_coor &last_body_part);
    int choose_random_direction_initialisation(std::vector<int> &all_directions);
    t_coor  get_position_after_movement(t_coor last_body, int direction, t_coor &body);


};



#endif