#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "utils.hpp"
#include "Board.hpp"

class   Board; 

#define ABSCISCA 0
#define ORDINATE 1

class Snake
{
public:
    Snake(Board &board, int &snake_size);
    ~Snake();

    // variables
    Board   &board;
    std::deque<t_coor>   _position;
    std::array< std::vector<char>, 4 >  vision;
    int     dir;

    // functions
    t_coor  get_head_position(void);
    void    print_dir(void);
    void    update_vision(void);
    void    print_vision(void);
    int     move(int direction);
    int     update_position_and_vision();


private:


    //private func
    int initialize_head(Board &board, int snake_size);
    int initialize_body(Board &board, int snake_size, int actual_size, t_coor &last_body_part);
    int choose_random_direction_initialisation(std::vector<int> &all_directions);
    t_coor  get_position_after_movement(t_coor last_body, int direction, t_coor &body);


};



#endif