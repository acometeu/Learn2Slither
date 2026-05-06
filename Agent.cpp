#include "include/Agent.hpp"

Agent::Agent(float epsilon, float alpha, float gamma): epsilon(epsilon), alpha(alpha), gamma(gamma){
    return;
}

Agent::~Agent(){
    return;
}

int     Agent::set_import_path(std::string import_path){

    
}

int     Agent::set_export_path(std::string export_path){


}


int    Agent::choose_direction(Snake &snake){
    
    float random = get_random_float(0, 1);
    if (random < this->epsilon)
        return (get_random_int(0, 3));
    else
        return(get_best_q_values_direction(snake));
}

int     Agent::get_best_q_values_direction(Snake &snake){
    //  get direction of higher q_value or if multiple best solutions, choose randomly between them
    
    std::vector<int>    all_dirs{LEFT, RIGHT, UP, DOWN};
    auto q_values = q_table[snake.get_snake_vision()];

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
