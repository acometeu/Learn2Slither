#include "include/Agent.hpp"

Agent::Agent(float epsilon, float alpha, float gamma): epsilon(epsilon), alpha(alpha), gamma(gamma){

    return;
}

Agent::~Agent(){

    if (ofs.is_open())
        ofs.close();
    return;
}

int     Agent::set_import_path(const std::string &import_path){

    ifs.open(import_path);
    if (!ifs.is_open())
    {
        std::cerr << "Failed opening import path : " << import_path << std::endl;
        return(1);
    }
    std::string line;
    while (std::getline(ifs, line))
        parse_and_add_q_values(line);

    return(0);
}

void    Agent::parse_and_add_q_values(const std::string &line){

    size_t delim = line.find_first_of(':');
    if (delim == std::string::npos)
        return;
    std::array<std::string, 4>  key = parse_q_table_key(line.substr(0, delim));
    std::array<float, 4>        values = parse_q_table_values(line.substr(delim + 1));
    q_table[key] = values;
}

std::array<std::string, 4> Agent::parse_q_table_key(const std::string &keys_line){

    std::vector<std::string>    vision = ft_split(keys_line, ',');
    std::array<std::string, 4>  key{};
    for (int i = 0; i < vision.size(); i++)
        key[i] = vision[i];
    return(key);
}

std::array<float, 4>    Agent::parse_q_table_values(const std::string &values_line){

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
            std::cout << "NO FIND" << std::endl;
            break;
        }
    }
    return (values);
}

int     Agent::set_export_path(const std::string &export_path){

    ofs.open(export_path, std::ofstream::trunc);
    if (!ofs.is_open())
    {
        std::cerr << "Failed opening export path : " << export_path << std::endl;
        return(1);
    }
    return(0);
}

int     Agent::save_q_table_to_export_path(void){

    if (!ofs.is_open())
    {
        std::cerr << "Saved to export file failed : stream closed" << std::endl;
        return (1);
    }

    auto q_table_end = q_table.end();
    for (auto it = q_table.begin(); it != q_table_end; it++)
    {
        auto key = (*it).first;
        auto values = (*it).second;
        ofs << key[LEFT] << ',' << key[RIGHT] << ',' << key[UP] << ',' << key[DOWN] << ':';
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

    if (!ofs.good())
    {
        std::cerr << "Saved to export file failed" << std::endl;
        return (1);
    }

    return(0);
}


int    Agent::choose_direction(Snake &snake){
    
    float random = get_random_float(0, 1);
    if (random < this->epsilon)
    {
        std::cout << "explooooooration" << std::endl;
        return (get_random_int(0, 3));
    }
    else
    {
        std::cout << "GREEEEEEEEEdy" << std::endl;
        return(get_best_q_values_direction(snake));
    }
}

int     Agent::get_best_q_values_direction(Snake &snake){
    //  get direction of higher q_value or if multiple best solutions, choose randomly between them
    
    std::vector<int>    all_dirs{LEFT, RIGHT, UP, DOWN};
    // auto q_values = q_table[snake.get_snake_vision()];
    auto it = q_table.find(snake.get_snake_vision());
    if (it == q_table.end())
        return (get_random_int(0, 3));
    auto q_values = (*it).second;

    int best_dir = get_random_int(0, 3);
    float max = q_values[best_dir];

    for (int i = 0; i < all_dirs.size(); i++)
    {
        if (q_values[i] > q_values[best_dir])
        {
            best_dir = i;
            max = q_values[best_dir];
        }
    }
    return (best_dir);
}

void    Agent::update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir){

    // std::cout << "old q_value = " << q_table[old_state][old_dir] << std::endl;
    // std::cout << "alpha = " << alpha << std::endl;
    // std::cout << "next max q_value = " << q_table[snake.get_snake_vision()][get_best_q_values_direction(snake)] << std::endl;
    // std::cout << "instance q_value = " << reward + (gamma * q_table[snake.get_snake_vision()][get_best_q_values_direction(snake)]) << std::endl;
    // std::cout << "alpha * (instance q_value + old q_value) = " << alpha * (reward + (gamma * q_table[snake.get_snake_vision()][get_best_q_values_direction(snake)]) - q_table[old_state][old_dir]) << std::endl;



    q_table[old_state][old_dir] += alpha * (reward + (gamma * q_table[snake.get_snake_vision()][get_best_q_values_direction(snake)] - q_table[old_state][old_dir]));
    // std::cout << "new q_value = " << q_table[old_state][old_dir] << std::endl;
    
}
