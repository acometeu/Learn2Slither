#include "Board.hpp"


Board::Board() : _board_size(12){
    inizialize_board();
    _snake = new Snake(*this);
}

Board::Board(int board_size) : _board_size(board_size + 2){
    inizialize_board();
    print_board();
    _snake = new Snake(*this);
}


Board::~Board(){
    delete _snake;
    return;
}

int Board::get_board_size(void){
    return(_board_size);
}


void    Board::inizialize_board(){
    for (int i = 0; i < _board_size; i++)
    {
        std::vector<char> initialize(_board_size, '0');
        map.push_back(initialize);
    }
   
    //  fill walls
    for (int i = 0; i  < _board_size; i++)
    {
        if (i == 0 || i == _board_size - 1)
        {
            for (int j = 0; j < _board_size; j++)
                map[i][j] = 'W';
        }
        else
        {
            map[i][0] = 'W';
            map[i][_board_size - 1] = 'W';
        }
    }
}

void    Board::print_board(void){
    for (int i = 0; i < _board_size; i++)
    {
        for (int j = 0; j < _board_size; j++)
            std::cout << map[i][j];
        std::cout << std::endl;
    }
}
