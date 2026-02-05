#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
class   Snake;
#include "Snake.hpp"


class Board
{
public:
    Board();
    Board(int board_size);
    ~Board();

    // accessor
    int get_board_size(void);


    // values
    std::vector< std::vector<char> > map;


    // functions
    void    print_board(void);


private:
    int     _board_size;
    Snake   *_snake;


    // functions
    void    inizialize_board();




};



#endif