#ifndef BOARD_HPP
#define BOARD_HPP

#include "utils.hpp"


#define HEAD 'H'
#define SNAKE 'S'
#define EMPTY '0'
#define WALL 'W'
#define GREEN_APPLE 'G'
#define RED_APPLE 'R'
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
    int green_apple;
    int red_apple;


    // functions
    void    print_board(void);
    bool    map_coor_is_empty(t_coor &coor);
    void    set_map_coor(t_coor &coor, char object);
    char    get_map_char(t_coor &coor);
    int     spawn_object(char object);


private:
    int     _board_size;


    // functions
    void    inizialize_board();
    void    inizialize_fruits();
    void    print_wall_line(void);





};



#endif