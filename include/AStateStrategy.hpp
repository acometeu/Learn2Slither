#ifndef ASTATESTRATEGY_HPP
#define ASTATESTRATEGY_HPP

#include "utils.hpp"

class AStateStrategy
{
public:
    virtual ~AStateStrategy() {};
    virtual uint32_t    encode_q_table(std::array<std::string, 4> const &vision) const = 0;
    virtual const std::vector<int>         &encode_dqn(std::array<std::string, 4> const &vision) const = 0;
    virtual int         get_dqn_input_number(void) const = 0;
};


#endif