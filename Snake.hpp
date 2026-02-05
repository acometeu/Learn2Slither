#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <list>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Board.hpp"
#include "utils.hpp"

class   Board; 
// t_coor;


class Snake
{
public:
    Snake(Board &board);
    ~Snake();



    private:
    std::list<t_coor>   _position;
    int initialize_head(Board &board);
    int initialize_body(Board &board, int snake_size, int actual_size);


};



#endif