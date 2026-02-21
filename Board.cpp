#include "include/Board.hpp"


Board::Board() : _board_size(12){
    inizialize_board();
}

Board::Board(int board_size) : _board_size(board_size + 2){
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
    if (coor.y <= 0 || coor.x <= 0 || coor.y > _board_size - 2 || coor.x > _board_size - 2)
        return(NAN);
    return (map[coor.y][coor.x]);
}




void    Board::inizialize_board(){
    for (int y = 0; y < _board_size; y++)
    {
        std::vector<char> initialize(_board_size, EMPTY);
        map.push_back(initialize);
    }
    fill_wall();
}

void    Board::fill_wall(){
    for (int y = 0; y  < _board_size; y++)
    {
        if (y == 0 || y == _board_size - 1)
        {
            for (int x = 0; x < _board_size; x++)
                map[y][x] = WALL;
        }
        else
        {
            map[y][0] = WALL;
            map[y][_board_size - 1] = WALL;
        }
    }
}

void    Board::print_board(void){
    for (int y = 0; y < _board_size; y++)
    {
        for (int x = 0; x < _board_size; x++)
            std::cout << map[y][x];
        std::cout << std::endl;
    }
}

bool    Board::map_coor_is_empty(t_coor &coor){
    if (map[coor.y][coor.x] == '0')
        return (true);
    return (false);
}

