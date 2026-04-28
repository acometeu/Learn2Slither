#include "include/Snake.hpp"

Snake::Snake(Board  &board, int &snake_size) : board(board), initial_size(snake_size), _stats(t_statistics()){
    // generate a pseudo random number generator
    srand(time(NULL));
    dir = rand() % 4;
    if (initialize_snake())
        throw std::invalid_argument("Not enough space for Snake on the Board");
    update_vision();
}

Snake::~Snake(){
    return;    
}

int Snake::initialize_snake(){

    //save impossible position for snake
    std::vector<t_coor> impossible_position;

    while (true)
    {
        if (!board.empty_cells.size())
            return(1);

        t_coor  head = board.get_random_empty_cell();

        _position.push_back(head);
        board.set_map_coor(head, HEAD);
        stats_add_length(_position.size());
        
        if (initialize_body(1, head))
        {
            _position.pop_back();
            board.set_map_coor(head, EMPTY);
            stats_reduce_length();
            impossible_position.push_back(head);
            continue;
        }

        //return impossible_position in empty_cells
        for (int i = 0; i < impossible_position.size(); i++)
            board.empty_cells.insert(impossible_position[i].y * board.get_board_size() + impossible_position[i].x);
        break;
    }
    
    return (0);
}

int Snake::initialize_body(int actual_size, t_coor &last_body){

    if (actual_size >= initial_size)
        return(0);

    std::vector<int>    all_directions = {UP, LEFT, RIGHT, DOWN};
    while (all_directions.size())
    {
        int direction = get_random_direction(all_directions);
        all_directions.pop_back();

        t_coor  new_body = get_position_after_movement(last_body, direction, new_body);
        if (board.get_map_char(new_body) != EMPTY)
            continue;
        
        //  body initialisation success
        _position.push_back(new_body);
        board.set_map_coor(new_body, SNAKE);
        stats_add_length(_position.size());
        if (initialize_body(actual_size + 1, new_body))
        {
            _position.pop_back();
            board.set_map_coor(new_body, EMPTY);
            stats_reduce_length();
            if (all_directions.size())
                continue;
            return(1);
        }
        return(0);
    }
    return(1);
}

int Snake::get_random_direction(std::vector<int> &all_directions){

        size_t random_index = rand() % all_directions.size();
        std::swap(all_directions[random_index], all_directions.back());
        return (all_directions.back());
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

    t_coor  head(0, 0);
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

    t_coor  head(0, 0);
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

    stats_add_turn();
    t_coor new_head = get_position_after_movement(_position.front(), direction, new_head);
    t_coor last_body = _position.back();

    board.set_map_coor(_position.front(), SNAKE);
    board.set_map_coor(_position.back(), EMPTY);
    _position.pop_back();
    
    switch (board.get_map_char(new_head))
    {
        case EMPTY :
        {
            board.set_map_coor(new_head, HEAD);
            _position.push_front(new_head);
            break;
        }
        case GREEN_APPLE :
        {
            stats_add_length(_position.size() + 1);
            board.set_map_coor(new_head, HEAD);
            _position.push_front(new_head);
            board.set_map_coor(last_body, SNAKE);
            _position.push_back(last_body);
            if (board.spawn_object(GREEN_APPLE))
            {
                std::cout << "FINISH" << std::endl;
                return (1);
            }
            break;
        }
        case RED_APPLE :
        {
            stats_reduce_length();
            board.set_map_coor(new_head, HEAD);
            _position.push_front(new_head);
            board.set_map_coor(_position.back(), EMPTY);
            _position.pop_back();
            if (!_position.size() || board.spawn_object(RED_APPLE))
                return(1);
            break;
        }
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

    print_dir();
    if (move(dir))
        return (1);
    update_vision();
    print_vision();
    return(0);
}

int Snake::reset(){

    _position.clear();
    if (initialize_snake())
        return(1);
    update_vision();

    return(0);
}

void    Snake::stats_add_turn(void){

    _stats.turns++;
}

void    Snake::stats_add_session(void){

    _stats.sessions++;
}

void    Snake::stats_add_length(int new_size){
    //take the new snake size

    //testsuppr
    std::cout << "adding, size : " << _position.size() << std::endl;

    _stats.total_length++;
    if (new_size > _stats.max_length)
        _stats.max_length = new_size;
}

void    Snake::stats_reduce_length(void){

    _stats.total_length--;
}

void    Snake::display_stats(void){

    std::cout << "--- Snake Stats ---" << std::endl;
    std::cout << "Sessions : " << _stats.sessions << std::endl;
    std::cout << "Turns : " << _stats.turns << std::endl;
    std::cout << "Average length : " << float(_stats.total_length) / float(_stats.sessions) << std::endl;
    std::cout << "Max length : " << _stats.max_length << std::endl;
    std::cout << "total length : " << _stats.total_length << std::endl;
}
