#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
// class   Snake;
// #include "Snake.hpp"
#include "utils.hpp"


#define HEAD 'H'
#define SNAKE 'S'
#define EMPTY '0'
#define WALL 'W'
#define NAN 'N'


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
    bool    map_coor_is_empty(t_coor &coor);
    void    set_map_coor(t_coor &coor, char object);
    char    get_map_char(t_coor &coor);


private:
    int     _board_size;


    // functions
    void    inizialize_board();
    void    fill_wall();




};



#endif