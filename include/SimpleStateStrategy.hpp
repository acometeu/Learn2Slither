#ifndef SIMPLESTATESTRATEGY_HPP
#define SIMPLESTATESTRATEGY_HPP

#include "AStateStrategy.hpp"

typedef struct simple_state
{
    bool    green_apple;
    bool    red_apple;
    bool    obstacle;

} t_simple_state;


class SimpleStateStrategy : public AStateStrategy
{
public:
    SimpleStateStrategy();
    ~SimpleStateStrategy();

    int encode(std::array<std::string, 4> const &vision) const override;
    t_simple_state  get_simple_state(const std::string &vision) const;
};



#endif