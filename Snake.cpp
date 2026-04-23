#include "include/Snake.hpp"

Snake::Snake(Board  &board, int &snake_size) : board(board){
    // generate a pseudo random number generator
    srand(time(NULL));
    dir = rand() % 4;
    initialize_head(board, snake_size);
}

Snake::~Snake(){
    return;    
}

int Snake::initialize_head(Board &board, int snake_size){
    t_coor  head;
    while (true)
    {
        head.x = rand() % (board.get_board_size());
        head.y = rand() % (board.get_board_size());

        if (board.map_coor_is_empty(head) == false)
            continue;

        _position.push_back(head);
        board.set_map_coor(head, 'H');
        if (initialize_body(board, snake_size, 1, head))
        {
            _position.pop_back();
            board.set_map_coor(head, '0');
            continue;
        }
        break;
    }
    
    return (0);
}

int Snake::initialize_body(Board &board, int snake_size, int actual_size, t_coor &last_body){

    if (actual_size >= snake_size)
        return(0);

    std::vector<int>    all_directions = {UP, LEFT, RIGHT, DOWN};
    while (all_directions.size())
    {
        int direction = choose_random_direction_initialisation(all_directions);
        all_directions.pop_back();

        t_coor  new_body = get_position_after_movement(last_body, direction, new_body);
        if (board.get_map_char(new_body) != '0')
            continue;
        
        //  body initialisation success
        _position.push_back(new_body);
        board.set_map_coor(new_body, 'S');
        if (initialize_body(board, snake_size, actual_size + 1, new_body))
        {
            _position.pop_back();
            board.set_map_coor(new_body, '0');
            if (all_directions.size())
                continue;
            return(1);
        }
        return(0);
    }
    return(1);
}

int Snake::choose_random_direction_initialisation(std::vector<int> &all_directions){

        size_t random_index = rand() % all_directions.size();
        //  swap random_index with last element if random_index is not last element
        if (random_index != all_directions.size() - 1)
            std::swap(all_directions[random_index], all_directions[all_directions.size() - 1]);
        return (all_directions[all_directions.size() - 1]);
}

t_coor  Snake::get_head_position(void){

    return(_position[0]);
}

void    Snake::print_dir(void){

    switch (dir)
    {
    case LEFT:
        std::cout << "LEFT" << std::endl;
        break;
    case RIGHT:
        std::cout << "RIGHT" << std::endl;
        break;
    case UP:
        std::cout << "UP" << std::endl;
        break;
    case DOWN:
        std::cout << "DOWN" << std::endl;
        break;
    }
    std::cout << std::endl;
}

void    Snake::update_vision(void){
    //  update vision with an array for each direction LEFT, RIGHT, UP, DOWN

    // clear previous values
    vision[LEFT].clear();
    vision[RIGHT].clear();
    vision[UP].clear();
    vision[DOWN].clear();

    t_coor  head;
    head.x = _position[0].x;
    head.y = _position[0].y;

    //fill vision LEFT
    for (int i = head.x - 1; i >= 0; i--)
        vision[LEFT].push_back(board.map[head.y][i]);
    //fill vision RIGHT
    for (int i = head.x + 1; i < board.get_board_size(); i++)
        vision[RIGHT].push_back(board.map[head.y][i]);
    //fill vision UP
    for (int i = head.y - 1; i >= 0; i--)
        vision[UP].push_back(board.map[i][head.x]);
    //fill vision DOWN
    for (int i = head.y + 1; i < board.get_board_size(); i++)
        vision[DOWN].push_back(board.map[i][head.x]);

}

void    Snake::print_vision(void){

    t_coor  head;
    head.x = _position[0].x;
    head.y = _position[0].y;

    //Print vision UP
    for (int i = 0; i < vision[UP].size(); i++)
    {
        for (int j = 0; j < vision[LEFT].size(); j++)
            std::cout << ' ';
        std::cout << vision[UP][vision[UP].size() - 1 - i];
        for (int j = 0; j < vision[RIGHT].size(); j++)
            std::cout << ' ';
        std::cout << std::endl;
    }

    //Print vision LEFT and RIGHT
    for (int i = vision[LEFT].size() - 1; i >= 0; i--)
        std::cout << vision[LEFT][i];
    std::cout << HEAD;
    for (int i = 0; i < vision[RIGHT].size(); i++)
        std::cout << vision[RIGHT][i];
    std::cout << std::endl;

    //Print vision DOWN
    for (int i = 0; i < vision[DOWN].size(); i++)
    {
        for (int j = 0; j < vision[LEFT].size(); j++)
            std::cout << ' ';
        std::cout << vision[DOWN][i];
        for (int j = 0; j < vision[RIGHT].size(); j++)
            std::cout << ' ';
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int Snake::move(int direction){

    t_coor new_head = get_position_after_movement(_position[0], direction, new_head);
    t_coor last_body = _position[_position.size() - 1];

    // move snake and update board on last body part
    _position.emplace_front(new_head);
    board.set_map_coor(_position[_position.size() - 1], EMPTY);
    _position.pop_back();
    
    // analyse where head is and update board
    switch (board.get_map_char(new_head))
    {
    case EMPTY :
        board.set_map_coor(_position[0], HEAD);
        if (_position.size() > 1)
            board.set_map_coor(_position[1], SNAKE);
        break;
    case GREEN_APPLE :
        _position.emplace_back(last_body);
        board.set_map_coor(_position[0], HEAD);
        board.set_map_coor(_position[1], SNAKE);
        board.set_map_coor(_position[_position.size() - 1], SNAKE);
        if (board.spawn_object(GREEN_APPLE))
        {
            std::cout << "FINISH" << std::endl;
            return (1);
        }
        break;
    case RED_APPLE :
        if (_position.size() <= 1)
        {
            std::cout << "RED APPLE DEAD" << std::endl;
            return(1);
        }
        board.set_map_coor(_position[_position.size() - 1], EMPTY);
        _position.pop_back();
        board.set_map_coor(_position[0], HEAD);
        if (_position.size() > 1)
            board.set_map_coor(_position[1], SNAKE);
        if (board.spawn_object(RED_APPLE))
            return (1);
        break;
    case WALL :
    case SNAKE :
    case HEAD :
    default :
        return (1);
    }
    return (0);
}

t_coor  Snake::get_position_after_movement(t_coor last_body, int direction, t_coor &body){

    switch (direction)
    {
    case UP:
        body.x = last_body.x;
        body.y = last_body.y - 1;
        break;
    case DOWN:
        body.x = last_body.x;
        body.y = last_body.y + 1;
        break;
    case LEFT:
        body.x = last_body.x - 1;
        body.y = last_body.y;
        break;
    case RIGHT:
        body.x = last_body.x + 1;
        body.y = last_body.y;
        break;
    default:
        body.x = -1;
        body.y = -1;
        return(body);
    }
    return(body);
}

int Snake::update_position_and_vision(void){

    if (move(dir))
        return (1);
    print_dir();
    update_vision();
    print_vision();
    return(0);
}