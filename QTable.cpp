#include "include/QTable.hpp"

QTable::QTable(float alpha, float gamma, AStateStrategy *state): AQMethod(alpha, gamma, state){

    return;
}

QTable::~QTable(){
    delete _state;
    return;
}

int QTable::set_q_values(std::ifstream &ifs){

    std::string line;
    while (std::getline(ifs, line))
    {
        size_t delim = line.find_first_of(':');
        if (delim == std::string::npos)
            return (1);
        int  key = std::stoi(line.substr(0, delim));
        std::array<float, 4>        values = parse_q_table_values(line.substr(delim + 1));
        q_table[key] = values;
    }
    return(0);
}


std::array<float, 4>    QTable::parse_q_table_values(const std::string &values_line){

    std::vector<std::string>    values_str = ft_split(values_line, ',');
    std::array<float, 4>       values{};

    for (int i = 0; i < values_str.size(); i++)
    {
        if (!values_str[i].size())
            continue;
        switch (values_str[i][0] - 48)
        {
        case LEFT:
            values[LEFT] = std::stof(values_str[i].substr(1));
            break;
        case RIGHT:
            values[RIGHT] = std::stof(values_str[i].substr(1));
            break;
        case UP:
            values[UP] = std::stof(values_str[i].substr(1));
            break;
        case DOWN:
            values[DOWN] = std::stof(values_str[i].substr(1));
            break;
        default:
            std::cout << "One invalid value found in import path !" << std::endl;
            break;
        }
    }
    return (values);
}

int     QTable::save_q_values(std::ofstream &ofs) const{

    std::unordered_map<int, std::array<float, 4>>::const_iterator q_table_end = q_table.end();
    for (std::unordered_map<int, std::array<float, 4>>::const_iterator it = q_table.begin(); it != q_table_end; it++)
    {
        int                     key = (*it).first;
        std::array<float, 4>    values = (*it).second;
        ofs << key << ':';
        if (values[LEFT])
            ofs << LEFT << values[LEFT] << ',';
        if (values[RIGHT])
            ofs << RIGHT << values[RIGHT] << ',';
        if (values[UP])
            ofs << UP << values[UP] << ',';
        if (values[DOWN])
            ofs << DOWN << values[DOWN];
        ofs << std::endl;
    }

    return(0);
}

int     QTable::get_best_q_values_direction(Snake &snake) const{
    //  get direction of higher q_value or if multiple best solutions, choose randomly between them
    
    std::vector<int>    all_dirs{LEFT, RIGHT, UP, DOWN};
    int key = _state->encode_q_table(snake.get_snake_vision());
    
    std::unordered_map<int, std::array<float, 4>>::const_iterator    it = q_table.find(key);
    if (it == q_table.end())
        return (get_random_int(0, 3));

    std::array<float, 4> q_values = (*it).second;
    int best_dir = get_random_int(0, 3);

    for (int i = 0; i < all_dirs.size(); i++)
    {
        if (q_values[i] > q_values[best_dir])
            best_dir = i;
    }
    return (best_dir);
}

void    QTable::update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir){

    int old_key = _state->encode_q_table(old_state);
    int new_key = _state->encode_q_table(snake.get_snake_vision());
    // std::cout << "old q_value = " << q_table[old_key][old_dir] << std::endl;
    // q_table[old_key][old_dir] += alpha * (reward + (gamma * q_table[new_key][get_best_q_values_direction(snake)] - q_table[old_key][old_dir]));
    q_table[old_key][old_dir] = q_table[old_key][old_dir] + (_alpha * (reward + _gamma * q_table[new_key][get_best_q_values_direction(snake)] - q_table[old_key][old_dir]));
    // std::cout << "new q_value = " << q_table[old_key][old_dir] << std::endl;
    
}