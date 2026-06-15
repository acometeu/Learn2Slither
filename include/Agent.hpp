#ifndef AGENT_HPP
#define AGENT_HPP

#include "utils.hpp"
#include "Snake.hpp"
#include "AStateStrategy.hpp"
#include "SimpleStateStrategy.hpp"
#include "IntermediateStateStrategy.hpp"
#include "ComplexStateStrategy.hpp"


class Agent
{
public:
    Agent(float epsilon, float alpha, float gamma, int total_sessions, AStateStrategy *strat);
    ~Agent();

    // variables
    float   epsilon; //exploration_rate
    float   alpha; //learning_rate
    float   gamma; //future_reward_significance
    AStateStrategy  *strategy; //strategy used to define state of q_table
    

    std::unordered_map<int, std::array<float, 4>>  q_table;

    // functions
    int     set_import_path(const std::string &import_path);
    int     set_export_path(const std::string &export_path);
    int     save_q_table_to_export_path(void);
    int     choose_direction(Snake &snake, MyArgs &args, int current_session);
    int     get_safe_random_q_value(Snake &snake);
    int     get_best_q_values_direction(Snake &snake);
    void    update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir);


private:

    // variables
    std::ofstream   _ofs;
    std::ifstream   _ifs;
    int             _total_session;

    // functions
    void    parse_and_add_q_values(const std::string &line);
    std::array<float, 4>        parse_q_table_values(const std::string &values_line);

};



#endif