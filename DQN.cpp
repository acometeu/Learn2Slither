#include "include/DQN.hpp"

DQN::DQN(float alpha, float gamma, AStateStrategy *state): AQMethod(alpha, gamma, state), _hidden_layer_nbr(2), _node_per_hidden_layer(8){

    initialize_neural_network();
    return;
}


DQN::~DQN(){
    delete _state;
    return;
}

void    DQN::initialize_neural_network(void){

    //initialize first layer
    if (_hidden_layer_nbr == 0)
        _first_layer_node_number = OUTPUT_NBR;
    else
        _first_layer_node_number = _node_per_hidden_layer;

    Eigen::MatrixXf first_weights_layer = Eigen::MatrixXf::Random(_first_layer_node_number, _state->get_dqn_input_number()) * 10;
    dqn_weights.push_back(first_weights_layer);
    Eigen::MatrixXf first_bias_layer = Eigen::MatrixXf::Constant(_first_layer_node_number, _state->get_dqn_input_number(), 0);
    dqn_bias.push_back(first_bias_layer);


    //initialize hidden layers
    for (int i = 1; i < _hidden_layer_nbr; i++)
    {
        Eigen::MatrixXf hidden_weights_layer = Eigen::MatrixXf::Random(_node_per_hidden_layer, _node_per_hidden_layer) * 10;
        dqn_weights.push_back(hidden_weights_layer);
        Eigen::MatrixXf hidden_bias_layer = Eigen::MatrixXf::Constant(_node_per_hidden_layer, _node_per_hidden_layer, 0);
        dqn_bias.push_back(hidden_bias_layer);
    }


    //initialize last layer
    if (_hidden_layer_nbr == 0)
    {
        //testsuppr
        print_dqn_weights();
        print_dqn_bias();
        return;
    }

    Eigen::MatrixXf last_weights_layer = Eigen::MatrixXf::Random(OUTPUT_NBR, _node_per_hidden_layer) * 10;
    dqn_weights.push_back(last_weights_layer);
    Eigen::MatrixXf last_bias_layer = Eigen::MatrixXf::Constant(OUTPUT_NBR, _node_per_hidden_layer, 0);
    dqn_bias.push_back(last_bias_layer);

    //testsuppr
    print_dqn_weights();
    print_dqn_bias();
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
    
    std::vector<int>    all_dirs{LEFT, RIGHT, UP, DOWN};
    const std::vector<int>  key = _state->encode_dqn(snake.get_snake_vision());
    
    // const std::array<int, 4>  output = get_dqn_output(key);






//     std::unordered_map<int, std::array<float, 4>>::const_iterator    it = q_table.find(key);
//     if (it == q_table.end())
//         return (get_random_int(0, 3));

//     std::array<float, 4> q_values = (*it).second;
//     int best_dir = get_random_int(0, 3);
//     float max = q_values[best_dir];

//     for (int i = 0; i < all_dirs.size(); i++)
//     {
//         if (q_values[i] > q_values[best_dir])
//         {
//             best_dir = i;
//             max = q_values[best_dir];
//         }
//     }
//     return (best_dir);
    return(0);
}

// const std::array<int, 4>    DQN::get_dqn_output(const std::vector<int> key) const{


// }


void    DQN::update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir){

    // int old_key = _state->encode(old_state);
    // int new_key = _state->encode(snake.get_snake_vision());
    // // std::cout << "old q_value = " << q_table[old_key][old_dir] << std::endl;
    // // q_table[old_key][old_dir] += alpha * (reward + (gamma * q_table[new_key][get_best_q_values_direction(snake)] - q_table[old_key][old_dir]));
    // q_table[old_key][old_dir] = q_table[old_key][old_dir] + (_alpha * (reward + _gamma * q_table[new_key][get_best_q_values_direction(snake)] - q_table[old_key][old_dir]));
    // // std::cout << "new q_value = " << q_table[old_key][old_dir] << std::endl;
    
}

void    DQN::print_dqn_weights(void){

    std::cout << "DQN Weights : " << std::endl;
    for (int i = 0; i < dqn_weights.size(); i++)
    {
        std::cout << "Cols = " << dqn_weights[i].cols() << ", Rows = " << dqn_weights[i].rows() << ", Size " << dqn_weights[i].size() << std::endl;
        std::cout << dqn_weights[i] << std::endl;
    }
}

void    DQN::print_dqn_bias(void){

    std::cout << "DQN Bias : " << std::endl;
    for (int i = 0; i < dqn_bias.size(); i++)
    {
        std::cout << "Cols = " << dqn_bias[i].cols() << ", Rows = " << dqn_bias[i].rows() << ", Size " << dqn_bias[i].size() << std::endl;
        std::cout << dqn_bias[i] << std::endl;
    }
}
