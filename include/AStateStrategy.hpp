#ifndef ASTATESTRATEGY_HPP
#define ASTATESTRATEGY_HPP

#include "utils.hpp"

class AStateStrategy
{
public:
    virtual ~AStateStrategy() {};
    virtual uint32_t encode(std::array<std::string, 4> const &vision) const = 0;
};


#endif