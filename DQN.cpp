#include "include/DQN.hpp"

DQN::DQN(float alpha, float gamma, AStateStrategy *state): AQMethod(alpha, gamma, state), hidden_layer_nbr(2), node_per_hidden_layer(8){

    initialize_neural_network();
    return;
}


DQN::~DQN(){
    delete _state;
    return;
}

void    DQN::initialize_neural_network(void){

    //initialize first layer
    int first_layer_node_number;
    if (hidden_layer_nbr == 0)
        first_layer_node_number = OUTPUT_NBR;
    else
        first_layer_node_number = node_per_hidden_layer;

    std::vector< std::vector<float> >   first_layer;
    for (int i = 0; i < first_layer_node_number; i++)
    {
        std::vector<float>  node_weights;
        int weight_nbr_per_node = _state->get_dqn_input_number();
        for (int j = 0; j < weight_nbr_per_node; j++)
            node_weights.push_back(get_random_float(-10, 10));
        first_layer.push_back(node_weights);
    }
    dqn_weights.push_back(first_layer);


    //initialize hidden layer
    for (int i = 1; i < hidden_layer_nbr; i++)
    {
        std::vector< std::vector<float> >   hidden_layer;
        for (int j = 0; j < node_per_hidden_layer; j++)
        {
            std::vector<float>  node;
            for (int k = 0; k < node_per_hidden_layer; k++)
                node.push_back(get_random_float(-10, 10));
            hidden_layer.push_back(node);
        }
        dqn_weights.push_back(hidden_layer);
    }


    //initialize last layer
    if (hidden_layer_nbr == 0)
        return;

    std::vector< std::vector<float> >   last_layer;
    for (int i = 0; i < OUTPUT_NBR; i++)
    {
        std::vector<float>  node;
        for (int j = 0; j < node_per_hidden_layer; j++)
            node.push_back(get_random_float(-10, 10));
        last_layer.push_back(node);
    }
    dqn_weights.push_back(last_layer);


    //testsuppr
    print_dqn();
}

int DQN::set_q_values(std::ifstream &ifs){

    // std::string line;
    // while (std::getline(ifs, line))
    // {
    //     size_t delim = line.find_first_of(':');
    //     if (delim == std::string::npos)
    //         return (1);
    //     int  key = std::stoi(line.substr(0, delim));
    //     std::array<float, 4>        values = parse_q_table_values(line.substr(delim + 1));
    //     q_table[key] = values;
    // }
    return(0);
}


std::array<float, 4>    DQN::parse_q_table_values(const std::string &values_line){

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

int     DQN::save_q_values(std::ofstream &ofs) const{

    // std::unordered_map<int, std::array<float, 4>>::const_iterator q_table_end = q_table.end();
    // for (std::unordered_map<int, std::array<float, 4>>::const_iterator it = q_table.begin(); it != q_table_end; it++)
    // {
    //     int                     key = (*it).first;
    //     std::array<float, 4>    values = (*it).second;
    //     ofs << key << ':';
    //     if (values[LEFT])
    //         ofs << LEFT << values[LEFT] << ',';
    //     if (values[RIGHT])
    //         ofs << RIGHT << values[RIGHT] << ',';
    //     if (values[UP])
    //         ofs << UP << values[UP] << ',';
    //     if (values[DOWN])
    //         ofs << DOWN << values[DOWN];
    //     ofs << std::endl;
    // }

    return(0);
}

int     DQN::get_best_q_values_direction(Snake &snake) const{
    //  get direction of higher q_value or if multiple best solutions, choose randomly between them
    
    // std::vector<int>    all_dirs{LEFT, RIGHT, UP, DOWN};
    // int key = _state->encode(snake.get_snake_vision());
    
    // std::unordered_map<int, std::array<float, 4>>::const_iterator    it = q_table.find(key);
    // if (it == q_table.end())
    //     return (get_random_int(0, 3));

    // std::array<float, 4> q_values = (*it).second;
    // int best_dir = get_random_int(0, 3);
    // float max = q_values[best_dir];

    // for (int i = 0; i < all_dirs.size(); i++)
    // {
    //     if (q_values[i] > q_values[best_dir])
    //     {
    //         best_dir = i;
    //         max = q_values[best_dir];
    //     }
    // }
    // return (best_dir);
    return(0);
}

void    DQN::update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir){

    // int old_key = _state->encode(old_state);
    // int new_key = _state->encode(snake.get_snake_vision());
    // // std::cout << "old q_value = " << q_table[old_key][old_dir] << std::endl;
    // // q_table[old_key][old_dir] += alpha * (reward + (gamma * q_table[new_key][get_best_q_values_direction(snake)] - q_table[old_key][old_dir]));
    // q_table[old_key][old_dir] = q_table[old_key][old_dir] + (_alpha * (reward + _gamma * q_table[new_key][get_best_q_values_direction(snake)] - q_table[old_key][old_dir]));
    // // std::cout << "new q_value = " << q_table[old_key][old_dir] << std::endl;
    
}

void    DQN::print_dqn(void){

    for (int i = 0; i < dqn_weights.size(); i++)
    {
        for (int j = 0; j < dqn_weights[i].size(); j++)
        {
            for(int k = 0; k < dqn_weights[i][j].size(); k++)
                std::cout << dqn_weights[i][j][k] << ' ';
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
