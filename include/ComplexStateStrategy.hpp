#ifndef COMPLEXSTATESTRATEGY_HPP
#define COMPLEXSTATESTRATEGY_HPP

#include "AStateStrategy.hpp"

typedef struct state_4_bools
{
    bool    green_apple;
    bool    red_apple;
    bool    body;
    bool    wall;

} t_state_4_bools;

typedef struct complex_state
{
    std::array<t_state_4_bools, 4>   visions;
    int     pos;    //position from 0 to 15 left to right, top to down
} t_complex_state;


class ComplexStateStrategy : public AStateStrategy
{
public:
    ComplexStateStrategy();
    ~ComplexStateStrategy();

    unsigned int encode(std::array<std::string, 4> const &vision) const override;
    unsigned int get_complex_state_pos(const std::array<std::string, 4> &vision) const;
    t_state_4_bools get_simple_state(const std::string &vision) const;
};



#endif