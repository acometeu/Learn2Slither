#include "include/DQN.hpp"

DQN::DQN(float alpha, float gamma, AStateStrategy *state): AQMethod(alpha, gamma, state), _hidden_layer_nbr(2), _node_per_hidden_layer(8), _replay_memory(500){

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
    Eigen::VectorXf first_bias_layer = Eigen::VectorXf::Constant(_first_layer_node_number, 1);
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
        Eigen::VectorXf hidden_bias_layer = Eigen::VectorXf::Constant(_node_per_hidden_layer, 2);
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
    Eigen::VectorXf last_bias_layer = Eigen::VectorXf::Constant(OUTPUT_NBR, 3);
    dqn_bias.push_back(last_bias_layer);
}

int DQN::set_q_values(std::ifstream &ifs){

    std::cout << "TESTTTTTT" << std::endl;
    std::string line;
    if (!std::getline(ifs, line))
        std::cerr << "Error: import file empty !" << std::endl;

    if (set_q_values_params(line))
        return(1);
    
    if (set_q_values_first_layer(ifs, line))
        return(1);
    if (set_q_values_hidden_layers(ifs, line))
        return(1);
    if (set_q_values_last_layer(ifs, line))
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

int     DQN::set_q_values_first_layer(std::ifstream &ifs, std::string &line){

    // discard weights matrice size line
    if (!std::getline(ifs, line))
        return (return_error_msg("Error: import file wrong format 1a"));

    for (int i = 0; i < _first_layer_node_number; i++)
    {
        if (!std::getline(ifs, line))
            return (return_error_msg("Error: import file wrong format 2a"));

        std::vector<std::string> matrice_row = ft_tokenize(line, ' ');
        for (int j = 0; j < _state->get_dqn_input_number(); j++)
        {
            if (j >= matrice_row.size())
                return (return_error_msg("Error: import file wrong format 3a"));
            dqn_weights[0](i, j) = std::stof(matrice_row[j]); 
        }
    }

    // discard bias vector size line
    if (!std::getline(ifs, line))
        return (return_error_msg("Error: import file wrong format 4a"));

    for (int i = 0; i < _first_layer_node_number; i++)
    {
        if (!std::getline(ifs, line))
            return (return_error_msg("Error: import file wrong format 5a"));
        dqn_bias[0](i) = std::stof(line);
    }
    return(0);
}

int     DQN::set_q_values_hidden_layers(std::ifstream &ifs, std::string &line){

    
    for (int k = 1; k < _hidden_layer_nbr; k++)
    {
        // discard weights matrice size line
        if (!std::getline(ifs, line))
            return (return_error_msg("Error: import file wrong format1b"));

        for (int i = 0; i < _node_per_hidden_layer; i++)
        {
            if (!std::getline(ifs, line))
                return (return_error_msg("Error: import file wrong format2b"));
    
            std::vector<std::string> matrice_row = ft_tokenize(line, ' ');
            for (int j = 0; j < _node_per_hidden_layer; j++)
                dqn_weights[k](i, j) = std::stof(matrice_row[j]); 
        }

        // discard bias vector size line
        if (!std::getline(ifs, line))
            return (return_error_msg("Error: import file wrong format 3b"));
    
        // std::vector<std::string> vector_row = ft_tokenize(line, ' ');
        for (int j = 0; j < _node_per_hidden_layer; j++)
        {
            if (!std::getline(ifs, line))
                return (return_error_msg("Error: import file wrong format 4b"));
            dqn_bias[k](j) = std::stof(line); 
        }
    }
    return(0);
}

int     DQN::set_q_values_last_layer(std::ifstream &ifs, std::string &line){

    if (_hidden_layer_nbr == 0)
        return (0);

    // discard weights matrice size line
    if (!std::getline(ifs, line))
        return (return_error_msg("Error: import file wrong format1c"));

    for (int i = 0; i < OUTPUT_NBR; i++)
    {
        if (!std::getline(ifs, line))
            return (return_error_msg("Error: import file wrong format2c"));

        std::vector<std::string> matrice_row = ft_tokenize(line, ' ');
        for (int j = 0; j < _node_per_hidden_layer; j++)
            dqn_weights[_hidden_layer_nbr](i, j) = std::stof(matrice_row[j]); 
    }

    // discard bias vector size line
    if (!std::getline(ifs, line))
        return (return_error_msg("Error: import file wrong format 3c"));

    for (int i = 0; i < OUTPUT_NBR; i++)
    {
        if (!std::getline(ifs, line))
            return (return_error_msg("Error: import file wrong format 4c"));
        dqn_bias[_hidden_layer_nbr](i) = std::stof(line); 
    }
    return(0);
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
    Eigen::VectorXf  key = _state->encode_dqn(snake.get_snake_vision());
    std::cout << "key = " << key << std::endl;

    
    std::array<int, 4>  q_values = get_q_values(key);
        //testsuppr
    std::cout <<"QVALUES" << std::endl;
    for (int i = 0; i < 4; i++)
    {
        std::cout << q_values[i] << ", ";
    }
    
    int best_dir = get_random_int(0, 3);

    for (int i = 0; i < all_dirs.size(); i++)
    {
        if (q_values[i] > q_values[best_dir])
            best_dir = i;
    }
    return (best_dir);
}

std::array<int, 4>    DQN::get_q_values(Eigen::VectorXf &node) const{

    // Eigen::VectorXf     node = dqn_weights[0] * key + dqn_bias[0];
    for (int i = 0; i <= _hidden_layer_nbr; i++)
    {
        std::cout << "dqn_weights cols = " << dqn_weights[i].cols() << ", rows = " << dqn_weights[i].rows() << std::endl;
        std::cout << "vector cols = " << node.cols() << ", rows = " << node.rows() << std::endl;

        node = dqn_weights[i] * node;// + dqn_bias[i];
    }
    return (eigen_vectorXf_to_output(node));





    // Eigen::VectorXf     first_node(dqn_weights[0].rows());
    // std::cout << "first node cols = " << dqn_weights[0].cols() << ", rows = " << dqn_weights[0].rows() << std::endl;
    // std::cout << "first vector cols = " << first_node.cols() << ", rows = " << first_node.rows() << std::endl;
    // //node after first dqn layer
    // first_node = dqn_weights[0] * key;// + dqn_bias[0];
    // std::cout << "test0" << std::endl;

    // if (_hidden_layer_nbr == 0)
    //     return (eigen_vectorXi_to_output(first_node));

    // // //node after hidden layers
    // Eigen::VectorXf hidden_layer_node(dqn_weights[1].rows());
    // for (int i = 1; i < _hidden_layer_nbr; i++)
    // {
    // std::cout << "test" << i << std::endl;
    // std::cout << "hidden node cols = " << dqn_weights[i].cols() << ", rows = " << dqn_weights[i].rows() << std::endl;
    // std::cout << "hidden vector cols = " << hidden_layer_node.cols() << ", rows = " << hidden_layer_node.rows() << std::endl;
        
    //     hidden_layer_node = dqn_weights[i] * hidden_layer_node;// + dqn_bias[i];
    // }

    // //node after last layer
    // Eigen::VectorXf last_node(dqn_weights[1].cols());
    //     std::cout << "testlast" << std::endl;
        
    //     last_node = dqn_weights[1] * key;// + dqn_bias[1];
    //     std::cout << "testlast+1" << std::endl;
    // return(eigen_vectorXi_to_output(last_node));
}

std::array<int, 4>  DQN::eigen_vectorXf_to_output(const Eigen::VectorXf &node) const{

    std::array<int, 4>  q_values;
    for (int i = 0; i < q_values.size(); i++)
        q_values[i] = node(i);
    return(q_values);
}


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
