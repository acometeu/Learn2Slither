#ifndef QTABLE_HPP
#define QTABLE_HPP

#include "AQMethod.hpp"


class QTable : public AQMethod
{
public:
    //variables
    std::unordered_map<int, std::array<float, 4>>  q_table;


    // functions
    QTable(float alpha, float gamma, AStateStrategy *state);
    ~QTable();
    virtual int     set_q_values(std::ifstream &ifs) override;
    virtual int     save_q_values(std::ofstream &ofs) const override;
    virtual int     get_best_q_values_direction(Snake &snake) const override;
    virtual void    update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir) override;    


private:
    // functions
    std::array<float, 4>        parse_q_table_values(const std::string &values_line);

};



#endif