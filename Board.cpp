#include "include/Board.hpp"


Board::Board() : _board_size(10){
    inizialize_board();
}

Board::Board(int board_size) : _board_size(board_size){
    inizialize_board();
}


Board::~Board(){
    return;
}

int Board::get_board_size(void){
    return(_board_size);
}

void    Board::set_map_coor(t_coor &coor, char object){
    map[coor.y][coor.x] = object;
}

char    Board::get_map_char(t_coor &coor){
    if (coor.y < 0 || coor.x < 0 || coor.y >= _board_size || coor.x >= _board_size)
        return(NAN);
    return (map[coor.y][coor.x]);
}




void    Board::inizialize_board(){
    for (int y = 0; y < _board_size; y++)
    {
        std::vector<char> initialize(_board_size, EMPTY);
        map.push_back(initialize);
    }
}

void    Board::print_board(void){

    print_wall_line();
    for (int y = 0; y < _board_size; y++)
    {
        std::cout << WALL;
        for (int x = 0; x < _board_size; x++)
            std::cout << map[y][x];
        std::cout << WALL;
        std::cout << std::endl;
    }
    print_wall_line();
}

void    Board::print_wall_line(void){
    for (int x = 0; x < _board_size + 2; x++)
        std::cout << WALL;
    std::cout << std::endl;
}

bool    Board::map_coor_is_empty(t_coor &coor){
    if (map[coor.y][coor.x] == '0')
        return (true);
    return (false);
}

