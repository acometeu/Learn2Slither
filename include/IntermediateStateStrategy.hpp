#ifndef INTERMEDIATESTATESTRATEGY_HPP
#define INTERMEDIATESTATESTRATEGY_HPP

#include "AStateStrategy.hpp"


typedef struct state_4_bools
{
    bool    green_apple;
    bool    red_apple;
    bool    body;
    bool    wall;

} t_state_4_bools;

typedef struct intermediate_state
{
    std::array<t_state_4_bools, 4>   visions;
    int     pos;    //position from 0 to 15 left to right, top to down
} t_intermediate_state;


class IntermediateStateStrategy : public AStateStrategy
{
public:
    IntermediateStateStrategy();
    ~IntermediateStateStrategy();

    uint32_t    encode_q_table(std::array<std::string, 4> const &vision) const override;
    std::vector<int>         encode_dqn(std::array<std::string, 4> const &vision) const override;
    int         get_dqn_input_number(void) const override;

private:
    uint32_t    get_intermediate_snake_pos(const std::array<std::string, 4> &vision) const;
    t_state_4_bools get_simple_state(const std::string &vision) const;
    void    state_push_x_and_y(std::vector<int> &state, const std::array<std::string, 4> &vision) const;
};



#endif