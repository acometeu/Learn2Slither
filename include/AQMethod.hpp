#ifndef AQMETHOD_HPP
#define AQMETHOD_HPP

#include "utils.hpp"
#include "Snake.hpp"
#include "AStateStrategy.hpp"
#include "SimpleStateStrategy.hpp"
#include "IntermediateStateStrategy.hpp"
#include "ComplexStateStrategy.hpp"


class AQMethod
{
public:

    // functions
    AQMethod(float alpha, float gamma, AStateStrategy *state): _alpha(alpha), _gamma(gamma), _state(state){};
    virtual ~AQMethod(){};
    virtual int     set_q_values(std::ifstream &ifs) = 0;
    virtual int     save_q_values(std::ofstream &ofs) const = 0;
    virtual int     get_best_q_values_direction(Snake &snake) const = 0;
    virtual void    update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir) = 0;

protected:
    //variables
    float   _alpha; //learning_rate
    float   _gamma; //future_reward_significance
    AStateStrategy  *_state; //strategy used to define state of q_table    

};



#endif