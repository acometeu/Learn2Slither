#include "include/IntermediateStateStrategy.hpp"

IntermediateStateStrategy::IntermediateStateStrategy(){

    return;
}

IntermediateStateStrategy::~IntermediateStateStrategy(){

    return;
}



uint32_t    IntermediateStateStrategy::encode(std::array<std::string, 4> const &vision) const {
    
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
