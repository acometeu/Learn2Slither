#include "include/IntermediateStateStrategy.hpp"

IntermediateStateStrategy::IntermediateStateStrategy(){

    return;
}

IntermediateStateStrategy::~IntermediateStateStrategy(){

    return;
}



uint32_t    IntermediateStateStrategy::encode_q_table(std::array<std::string, 4> const &vision) const {
    
    t_intermediate_state state;
    state.pos = get_intermediate_snake_pos(vision);
    uint32_t  hash = state.pos;
    for (int i = 0; i < 4; i++)
    {
        state.visions[i] = get_simple_state(vision[i]);
        hash = (hash << 1) + state.visions[i].green_apple;
        hash = (hash << 1) + state.visions[i].red_apple;
        hash = (hash << 1) + state.visions[i].body;
        hash = (hash << 1) + state.visions[i].wall;
    }
    return(hash);
}

uint32_t IntermediateStateStrategy::get_intermediate_snake_pos(const std::array<std::string, 4> &vision) const{
    
    float size = vision[0].size() + vision[1].size() + 1; //equivalent of get_board_size
    uint32_t pos = 0;
    pos += vision[2].size() / (size/4);
    pos <<= 2;
    pos += vision[0].size() / (size/4);
    return(pos);
}


t_state_4_bools  IntermediateStateStrategy::get_simple_state(const std::string &vision) const{

    t_state_4_bools state = {false, false, false, false};
    
    //check if first case is obstacle
    if (!vision.size())
    state.wall = true;
    else if (vision[0] == 'S')
    state.body = true;
    
    //check others cases
    for (int i = 0; i < vision.size(); i++)
    {
        switch (vision[i])
        {
            case 'G' :
            state.green_apple = true;
            break;
            case 'R' :
            state.red_apple = true;
            break;
            default:
            break;
        }
    }
    
    return(state);
}

std::vector<int>         IntermediateStateStrategy::encode_dqn(std::array<std::string, 4> const &visions) const{
    
    std::vector<int>    state;

    state_push_x_and_y(state, visions);

    for (int i = 0; i < 4; i++)
    {
        t_state_4_bools vision = get_simple_state(visions[i]);
        // state.visions[i] = get_simple_state(vision[i]);
        state.push_back(vision.green_apple);
        state.push_back(vision.red_apple);
        state.push_back(vision.body);
        state.push_back(vision.wall);
    }
    return(state);
}

void    IntermediateStateStrategy::state_push_x_and_y(std::vector<int> &state, const std::array<std::string, 4> &vision) const{

    float size = vision[0].size() + vision[1].size() + 1; //equivalent of get_board_size
    state.push_back(vision[2].size() / (size/4));
    state.push_back(vision[0].size() / (size/4));
}


int         IntermediateStateStrategy::get_dqn_input_number(void) const{
    //give the number of input this state strategy have for a neural network

    return(18); //4 bool for 4 direction + x and y coordinates (4*4+2)
}