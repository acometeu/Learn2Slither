#include "include/Board.hpp"


Board::Board() : _board_size(10), green_apple(2), red_apple(1){
    inizialize_board();
    inizialize_fruits();
}

Board::Board(int board_size) : _board_size(board_size), green_apple(2), red_apple(1){
    inizialize_board();
    inizialize_fruits();
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
        return(WALL);
    return (map[coor.y][coor.x]);
}


void    Board::inizialize_board(){
    for (int y = 0; y < _board_size; y++)
    {
        std::vector<char> initialize(_board_size, EMPTY);
        map.push_back(initialize);
    }
}

void    Board::inizialize_fruits(){

    std::cout << "test" << std::endl;
    srand(time(NULL));

    t_coor  apple;
    for (size_t i = 0; i < green_apple; i++)
    {
        while (true)
        {
            apple.x = rand() % (get_board_size());
            apple.y = rand() % (get_board_size());
            if (map_coor_is_empty(apple) == true)
            {
                map[apple.y][apple.x] = GREEN_APPLE; 
                break;
            }
        }
    }
    while (true)
    {
        apple.x = rand() % (get_board_size());
        apple.y = rand() % (get_board_size());
        if (map_coor_is_empty(apple) == true)
        {
            map[apple.y][apple.x] = RED_APPLE; 
            break;
        }
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

int     Board::spawn_object(char object){

    int random_y = rand() % (get_board_size());
    int random_x = rand() % (get_board_size());
    t_coor  coor;

    // check from random box until the end
    coor.y = random_y;
    coor.x = random_x;
    while (coor.y < get_board_size())
    {
        while (coor.x < get_board_size())
        {
            if (map_coor_is_empty(coor))
            {
                set_map_coor(coor, object);
                return(0);
            }
            coor.x++;
        }
        coor.x = 0;
        coor.y++;
    }

    // check from beginning until the random box
    coor.y = 0;
    while (coor.y <= random_y)
    {
        coor.x = 0;
        while(coor.x < get_board_size())
        {
            if (map_coor_is_empty(coor))
            {
                set_map_coor(coor, object);
                return(0);
            }
            coor.x++;
        }
        coor.y++;
    }
    return(1);
}