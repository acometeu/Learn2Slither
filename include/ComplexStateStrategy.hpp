#ifndef COMPLEXSTATESTRATEGY_HPP
#define COMPLEXSTATESTRATEGY_HPP

#include "AStateStrategy.hpp"
#include "IntermediateStateStrategy.hpp"

typedef struct complex_state
{
    std::array<t_state_4_bools, 4>   visions;
    int     pos;    //position from 0 to X (X = board_size * board_size) left to right, top to down
} t_complex_state;


class ComplexStateStrategy : public AStateStrategy
{
public:
    ComplexStateStrategy();
    ~ComplexStateStrategy();

    uint32_t    encode(std::array<std::string, 4> const &vision) const override;
    uint32_t    get_complex_snake_pos(const std::array<std::string, 4> &vision) const;
    t_state_4_bools get_simple_state(const std::string &vision) const;
};



#endif