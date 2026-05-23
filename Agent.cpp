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
    std::array<t_simple_q, 4>  key = parse_q_table_key(line.substr(0, delim));
    std::array<float, 4>        values = parse_q_table_values(line.substr(delim + 1));
    q_table[key] = values;
}

std::array<t_simple_q, 4> Agent::parse_q_table_key(const std::string &keys_line){

    std::vector<std::string>    vision = ft_split(keys_line, ',');
    std::array<t_simple_q, 4>  keys{};
    for (int i = 0; i < vision.size(); i++)
    {
        std::vector<std::string>    key = ft_split(vision[i], '.');
        if (key.size() != 3)
            std::cerr << "Error parsing import file : wrong number of key member" << std::endl;
        keys[i].obstacle = std::stoi(key[0]);
        keys[i].green_apple = std::stoi(key[1]);
        keys[i].red_apple = std::stoi(key[2]);
    }
    return(keys);
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
        ofs << key[LEFT].obstacle << '.' << key[LEFT].green_apple << '.' << key[LEFT].red_apple << ','
            << key[RIGHT].obstacle << '.' << key[RIGHT].green_apple << '.' << key[RIGHT].red_apple << ','
            << key[UP].obstacle << '.' << key[UP].green_apple << '.' << key[UP].red_apple << ','
            << key[DOWN].obstacle << '.' << key[DOWN].green_apple << '.' << key[DOWN].red_apple << ':';
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
    
    // exploration rate
    if (get_random_float(0, 1) < this->epsilon)
        return (get_random_int(0, 3));

    // random unstuck rate
    if (get_random_int(0, 49) == 0);
        return (get_safe_random_q_value(snake));
    
    // greedy
    return(get_best_q_values_direction(snake));
}

std::array<t_simple_q, 4>   Agent::get_t_q_table_key(const std::array<std::string, 4> &vision)
{
    std::array<t_simple_q, 4>    key{{{0,0,0},{0,0,0},{0,0,0},{0,0,0}}};
    for (int i = 0; i < 4; i++)
    {
        //check first case
        if (vision[i].size())
        {
            switch (vision[i][0])
            {
            case 'S':
                key[i].obstacle = true;
                break;
            }
        }
        else
            key[i].obstacle = true;

        //check others cases
        for (int j = 0; j < vision[i].size(); j++)
        {
            switch (vision[i][j])
            {
            case 'G' :
                if (!key[i].green_apple)
                    key[i].green_apple = j + 1;
                break;
            case 'R' :
                if (!key[i].red_apple)
                    key[i].red_apple = j + 1;
                break;
            default:
                break;
            }
        }
    }
    return(key);
}

int     Agent::get_safe_random_q_value(Snake &snake){
    // get random direction without direct obstacle (if possible)

    std::vector<int>    all_dirs{LEFT, RIGHT, UP, DOWN};
    auto vision = snake.get_snake_vision();
    for (int index_max = all_dirs.size() - 1; index_max > 0; index_max--)
    {
        int random = get_random_int(0, index_max);
        std::swap(all_dirs[index_max], all_dirs[random]);
        if (vision[all_dirs[index_max]].size() && vision[all_dirs[index_max]][0] != 'S')
            return (all_dirs[index_max]);
    }
    return(all_dirs[0]);
}

int     Agent::get_best_q_values_direction(Snake &snake){
    //  get direction of higher q_value or if multiple best solutions, choose randomly between them
    
    std::vector<int>    all_dirs{LEFT, RIGHT, UP, DOWN};
    auto vision = snake.get_snake_vision();
    std::array<t_simple_q, 4>   keys = get_t_q_table_key(vision);
    auto it = q_table.find(keys);
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

    auto old_key = get_t_q_table_key(old_state);
    auto new_key = get_t_q_table_key(snake.get_snake_vision());
    // std::cout << "old q_value = " << q_table[old_key][old_dir] << std::endl;
    q_table[old_key][old_dir] += alpha * (reward + (gamma * q_table[new_key][get_best_q_values_direction(snake)] - q_table[old_key][old_dir]));
    // std::cout << "new q_value = " << q_table[old_key][old_dir] << std::endl;
    
}
