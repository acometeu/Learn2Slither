#include "include/SimpleStateStrategy.hpp"

SimpleStateStrategy::SimpleStateStrategy(){

    return;
}

SimpleStateStrategy::~SimpleStateStrategy(){

    return;
}



int SimpleStateStrategy::encode(std::array<std::string, 4> const &vision) const {
    
    int  hash = 0;
    for (int i = 0; i < 4; i++)
    {
        t_simple_state  state = get_simple_state(vision[i]);
        hash = (hash << 1) + state.green_apple;
        hash = (hash << 1) + state.red_apple;
        hash = (hash << 1) + state.obstacle;
    }
    return(hash);
}

t_simple_state  SimpleStateStrategy::get_simple_state(const std::string &vision) const{

    t_simple_state state = {false, false, false};
        
    //check if first case is obstacle
    if (!vision.size() || vision[0] == 'S')
        state.obstacle = true;
    
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
