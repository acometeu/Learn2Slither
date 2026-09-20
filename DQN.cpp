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

    initialize_first_layer();
    initialize_hidden_layers();
    initialize_last_layer();

    //testsuppr
    print_dqn_weights();
    print_dqn_bias();
}

void    DQN::initialize_first_layer(void){

    if (_hidden_layer_nbr == 0)
        _first_layer_node_number = OUTPUT_NBR;
    else
        _first_layer_node_number = _node_per_hidden_layer;

    Eigen::MatrixXf first_weights_layer(_first_layer_node_number, _state->get_dqn_input_number());
    for (int i = 0; i < _first_layer_node_number; i++)
    {
        for (int j = 0; j < _state->get_dqn_input_number(); j++)
            first_weights_layer(i, j) = get_random_float(-10, 10);
    }
    dqn_weights.push_back(first_weights_layer);
    Eigen::MatrixXf first_bias_layer = Eigen::MatrixXf::Constant(_first_layer_node_number, _state->get_dqn_input_number(), 0);
    dqn_bias.push_back(first_bias_layer);
}

void    DQN::initialize_hidden_layers(){

    for (int i = 1; i < _hidden_layer_nbr; i++)
    {
        Eigen::MatrixXf hidden_weights_layer(_node_per_hidden_layer, _node_per_hidden_layer);
        for (int i = 0; i < _node_per_hidden_layer; i++)
        {
            for (int j = 0; j < _node_per_hidden_layer; j++)
                hidden_weights_layer(i, j) = get_random_float(-10, 10);
        }
        dqn_weights.push_back(hidden_weights_layer);
        Eigen::MatrixXf hidden_bias_layer = Eigen::MatrixXf::Constant(_node_per_hidden_layer, _node_per_hidden_layer, 0);
        dqn_bias.push_back(hidden_bias_layer);
    }
}

void    DQN::initialize_last_layer(void){

    if (_hidden_layer_nbr == 0)
    {
        //testsuppr
        print_dqn_weights();
        print_dqn_bias();
        return;
    }

    Eigen::MatrixXf last_weights_layer(OUTPUT_NBR, _node_per_hidden_layer);
    for (int i = 0; i < OUTPUT_NBR; i++)
    {
        for (int j = 0; j < _node_per_hidden_layer; j++)
            last_weights_layer(i, j) = get_random_float(-10, 10);
    }
    dqn_weights.push_back(last_weights_layer);
    Eigen::MatrixXf last_bias_layer = Eigen::MatrixXf::Constant(OUTPUT_NBR, _node_per_hidden_layer, 0);
    dqn_bias.push_back(last_bias_layer);
}

int DQN::set_q_values(std::ifstream &ifs){

    std::string line;
    if (!std::getline(ifs, line))
        std::cerr << "Error: import file empty !" << std::endl;

    if (set_q_values_params(line))
        return(1);
    
    if (set_q_values_first_layer(ifs, line, dqn_weights))
        return(1);
    if (set_q_values_first_layer(ifs, line, dqn_bias))
        return(1);
    if (set_q_values_hidden_layers(ifs, line, dqn_weights))
        return(1);
    if (set_q_values_hidden_layers(ifs, line, dqn_bias))
        return(1);
    if (set_q_values_last_layer(ifs, line, dqn_weights))
        return(1);
    if (set_q_values_last_layer(ifs, line, dqn_bias))
        return(1);

    return(0);
}

int DQN::set_q_values_params(const std::string &line){

    std::vector<std::string>    params = ft_split(line, ',');
    if (params.size() != 3)
        return (return_error_msg("Error: import file wrong params"));

    int first_param = std::stoi(params[0].substr(0));
    if (first_param != _state->get_dqn_input_number())
        return (return_error_msg("Error: import file wrong state strategy input nbr"));

    int second_param = std::stoi(params[1].substr(0));
    if (second_param != _hidden_layer_nbr)
        return (return_error_msg("Error: import file wrong hidden layer nbr"));

    int third_param = std::stoi(params[2].substr(0));
    if (third_param != _node_per_hidden_layer)
        return (return_error_msg("Error: import file wrong node per layer nbr"));

    return(0);
}

int     DQN::set_q_values_first_layer(std::ifstream &ifs, std::string &line, std::vector<Eigen::MatrixXf> &dqn){

    // discard weights matrice size line
    if (!std::getline(ifs, line))
        return (return_error_msg("Error: import file wrong format1"));

    for (int i = 0; i < _first_layer_node_number; i++)
    {
        if (!std::getline(ifs, line))
            return (return_error_msg("Error: import file wrong format2"));

        std::vector<std::string> matrice_row = ft_tokenize(line, ' ');
        for (int j = 0; j < _state->get_dqn_input_number(); j++)
            dqn[0](i, j) = std::stof(matrice_row[j]); 
    }
    return(0);
}

int     DQN::set_q_values_hidden_layers(std::ifstream &ifs, std::string &line, std::vector<Eigen::MatrixXf> &dqn){

    // discard weights matrice size line
    if (!std::getline(ifs, line))
        return (return_error_msg("Error: import file wrong format3"));

    for (int k = 1; k < _hidden_layer_nbr; k++)
    {
        for (int i = 0; i < _node_per_hidden_layer; i++)
        {
            if (!std::getline(ifs, line))
                return (return_error_msg("Error: import file wrong format4"));
    
            std::vector<std::string> matrice_row = ft_tokenize(line, ' ');
            for (int j = 0; j < _node_per_hidden_layer; j++)
                dqn[k](i, j) = std::stof(matrice_row[j]); 
        }
    }
    return(0);
}

int     DQN::set_q_values_last_layer(std::ifstream &ifs, std::string &line, std::vector<Eigen::MatrixXf> &dqn){

    if (_hidden_layer_nbr == 0)
        return (0);

    // discard weights matrice size line
    if (!std::getline(ifs, line))
        return (return_error_msg("Error: import file wrong format5"));

    for (int i = 0; i < OUTPUT_NBR; i++)
    {
        if (!std::getline(ifs, line))
            return (return_error_msg("Error: import file wrong format6"));

        std::vector<std::string> matrice_row = ft_tokenize(line, ' ');
        for (int j = 0; j < _node_per_hidden_layer; j++)
            dqn[_hidden_layer_nbr](i, j) = std::stof(matrice_row[j]); 
    }
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

    //save DQN size
    ofs << _state->get_dqn_input_number() << ',';
    ofs << _hidden_layer_nbr << ',';
    ofs << _node_per_hidden_layer << std::endl;

    //save DQN values
    for (int i = 0; i <= _hidden_layer_nbr; i++)
    {
        //rows, cols size
        ofs << dqn_weights[i].rows() << "," << dqn_weights[i].cols() << std::endl;
        ofs << dqn_weights[i] << std::endl; 
        ofs << dqn_bias[i].rows() << "," << dqn_bias[i].cols() << std::endl;
        ofs << dqn_bias[i] << std::endl; 
    }
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
