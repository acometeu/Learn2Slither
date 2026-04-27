#include "include/Board.hpp"


Board::Board() : _board_size(10), green_apple(2), red_apple(1){
    inizialize_board();
    initialize_empty_cells();
    if (inizialize_fruits())
        throw std::invalid_argument("Not enough space on the board");
}

Board::Board(int board_size) : _board_size(board_size), green_apple(2), red_apple(1){
    if (board_size > sqrt(__INT_MAX__))
        throw std::invalid_argument("Board size can't be more than square root of INT MAX");
    inizialize_board();
    initialize_empty_cells();
    if (inizialize_fruits())
        throw std::invalid_argument("Not enough space on the board");
}


Board::~Board(){
    return;
}

int Board::get_board_size(void){
    return(_board_size);
}

void    Board::set_map_coor(t_coor &coor, char object){
    map[coor.y][coor.x] = object;
    if (object == EMPTY)
        empty_cells.insert(coor.y * _board_size + coor.x);
    else
        empty_cells.erase(coor.y * _board_size + coor.x);
}

char    Board::get_map_char(t_coor &coor){
    if (coor.y < 0 || coor.x < 0 || coor.y >= _board_size || coor.x >= _board_size)
        return(WALL);
    return (map[coor.y][coor.x]);
}


void    Board::inizialize_board(){
    for (int y = 0; y < _board_size; y++)
    {
        std::string initialize(_board_size, EMPTY);
        map.push_back(initialize);
    }
}

void    Board::initialize_empty_cells(){

    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].size(); x++)
        {
            // t_coor  empty_cell(y, x);
            empty_cells.insert(y * _board_size + x);
        }
    }
}


int Board::inizialize_fruits(){

    srand(time(NULL));

    t_coor  apple(0, 0);
    for (size_t i = 0; i < green_apple; i++)
    {
        if (spawn_object(GREEN_APPLE))
            return(1);
    }
    for (size_t i = 0; i < red_apple; i++)
    {
        if (spawn_object(RED_APPLE))
            return(1);
    }
    return(0);
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

int     Board::spawn_object(char object){
    if (!empty_cells.size())
        return(1);

    t_coor empty_cell = get_random_empty_cell();
    set_map_coor(empty_cell, object);
    return(0);
}

t_coor  Board::get_random_empty_cell(void){
    auto it = empty_cells.begin();
    for (int i = rand() % empty_cells.size(); i > 0; i--)
        it++;
    t_coor empty_cell(*it / _board_size, *it % _board_size);
    return(empty_cell);
}