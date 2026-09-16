#ifndef AGENT_HPP
#define AGENT_HPP

#include "utils.hpp"
#include "Snake.hpp"
#include "AQMethod.hpp"
#include "QTable.hpp"
#include "DQN.hpp"
// #include "AStateStrategy.hpp"
// #include "SimpleStateStrategy.hpp"
// #include "IntermediateStateStrategy.hpp"
// #include "ComplexStateStrategy.hpp"


class Agent
{
public:
    Agent(float epsilon, float alpha, float gamma, int total_sessions, AQMethod *q_method, AStateStrategy *strat);
    ~Agent();

    // variables
    float   epsilon; //exploration_rate
    AQMethod    *q_method; //Q_learning method used (Q_table or DQN)
    


    // functions
    int     set_import_path(const std::string &import_path);
    int     set_export_path(const std::string &export_path);
    int     save_q_table_to_export_path(MyArgs &args);
    int     choose_direction(Snake &snake, MyArgs &args, int current_session);
    void    update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir);


private:

    // variables
    std::ofstream   _ofs;
    std::ifstream   _ifs;
    int             _total_session;
};



#endif