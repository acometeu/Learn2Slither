#include "include/Agent.hpp"

Agent::Agent(float epsilon, float alpha, float gamma, int total_session, AStateStrategy *strat): epsilon(epsilon), alpha(alpha), gamma(gamma), _total_session(total_session), strategy(strat){

    return;
}

Agent::~Agent(){

    if (_ofs.is_open())
        _ofs.close();
    delete strategy;
    return;
}

int     Agent::set_import_path(const std::string &import_path){

    _ifs.open(import_path);
    if (!_ifs.is_open())
    {
        std::cerr << "Failed opening import path : " << import_path << std::endl;
        return(1);
    }
    std::string line;
    while (std::getline(_ifs, line))
        parse_and_add_q_values(line);

    return(0);
}

void    Agent::parse_and_add_q_values(const std::string &line){

    size_t delim = line.find_first_of(':');
    if (delim == std::string::npos)
        return;
    int  key = std::stoi(line.substr(0, delim));
    std::array<float, 4>        values = parse_q_table_values(line.substr(delim + 1));
    q_table[key] = values;
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

    _ofs.open(export_path, std::ofstream::trunc);
    if (!_ofs.is_open())
    {
        std::cerr << "Failed opening export path : " << export_path << std::endl;
        return(1);
    }
    return(0);
}

int     Agent::save_q_table_to_export_path(MyArgs &args){

    if (args.export_path.empty())
        return(0);

    if (!_ofs.is_open())
    {
        std::cerr << "Saved to export file failed : stream closed" << std::endl;
        return (1);
    }

    std::unordered_map<int, std::array<float, 4>>::iterator q_table_end = q_table.end();
    for (std::unordered_map<int, std::array<float, 4>>::iterator it = q_table.begin(); it != q_table_end; it++)
    {
        int                     key = (*it).first;
        std::array<float, 4>    values = (*it).second;
        _ofs << key << ':';
        if (values[LEFT])
            _ofs << LEFT << values[LEFT] << ',';
        if (values[RIGHT])
            _ofs << RIGHT << values[RIGHT] << ',';
        if (values[UP])
            _ofs << UP << values[UP] << ',';
        if (values[DOWN])
            _ofs << DOWN << values[DOWN];
        _ofs << std::endl;
    }

    if (!_ofs.good())
    {
        std::cerr << "Saved to export file failed" << std::endl;
        return (1);
    }

    return(0);
}

//temptestsuppr
#define MAX_EXPLORATION 0
#define LITTLE_EXPLORATION 1 
#define NO_EXPLORATION 2

int    Agent::choose_direction(Snake &snake, MyArgs &args, int current_session){
    
    // exploration rate
    if (!args.no_learning)
    {
        if (current_session <= _total_session * 0.25) //~first quartile
        {
            if (get_random_float(0, 1) < epsilon)
                return(get_random_int(0, 3));
        }
    }

    return(get_best_q_values_direction(snake));
}

int     Agent::get_best_q_values_direction(Snake &snake){
    //  get direction of higher q_value or if multiple best solutions, choose randomly between them
    
    std::vector<int>    all_dirs{LEFT, RIGHT, UP, DOWN};
    std::array<std::string, 4>  vision = snake.get_snake_vision();
    int key = strategy->encode(vision);
    
    std::unordered_map<int, std::array<float, 4>>::iterator    it = q_table.find(key);
    if (it == q_table.end())
        return (get_random_int(0, 3));

    std::array<float, 4> q_values = (*it).second;
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

    int old_key = strategy->encode(old_state);
    int new_key = strategy->encode(snake.get_snake_vision());
    // std::cout << "old q_value = " << q_table[old_key][old_dir] << std::endl;
    q_table[old_key][old_dir] += alpha * (reward + (gamma * q_table[new_key][get_best_q_values_direction(snake)] - q_table[old_key][old_dir]));
    // std::cout << "new q_value = " << q_table[old_key][old_dir] << std::endl;
    
}