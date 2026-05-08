#ifndef AGENT_HPP
#define AGENT_HPP

#include "utils.hpp"
#include "Snake.hpp"


struct hash_qtable
{
    size_t  operator()(const std::array<std::string, 4> &key) const{
        return (std::hash<std::string>{}(key[0] + ',' + key[1] + ',' + key[2] + ',' + key[3]));
    }
};


class Agent
{
public:
    Agent(float epsilon, float alpha, float gamma);
    ~Agent();

    // variables
    float   epsilon; //exploration_rate
    float   alpha; //learning_rate
    float   gamma; //future_reward_significance

    std::unordered_map< std::array<std::string, 4>, std::array<float, 4>, hash_qtable>  q_table;

    // functions
    int     set_import_path(const std::string &import_path);
    int     set_export_path(const std::string &export_path);
    int     save_q_table_to_export_path(void);
    int     choose_direction(Snake &snake);
    int     get_best_q_values_direction(Snake &snake);
    void    update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir);


    // q_function methods
    // static void q_funtion_classic(void);

private:

    // variables
    std::ofstream   ofs;
    std::ifstream   ifs;

    // functions
    void    parse_and_add_q_values(const std::string &line);
    std::array<std::string, 4>  parse_q_table_key(const std::string &keys_line);
    std::array<float, 4>        parse_q_table_values(const std::string &values_line);

};



#endif