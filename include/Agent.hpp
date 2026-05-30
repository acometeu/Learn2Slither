#ifndef AGENT_HPP
#define AGENT_HPP

#include "utils.hpp"
#include "Snake.hpp"
#include "AStateStrategy.hpp"
#include "SimpleStateStrategy.hpp"


// typedef struct s_q_table_key
// {
//     bool    green_apple;
//     bool    red_apple;
//     bool    obstacle;

//     bool operator==(const s_q_table_key& key) const{
//         if (obstacle == key.obstacle && green_apple == key.green_apple && red_apple == key.red_apple)
//             return(true);
//         return(false);
//     }
// } t_q_table_key;

// struct hash_qtable
// {
//     int  operator()(const std::array<t_q_table_key, 4> &key) const{
//         int  hash = 0;
//         int key_size = key.size();
//         for (int i = 0; i < key_size; i++)
//         {
//             hash <<= 2;
//             hash += key[i].green_apple;
//             hash <<= 2;
//             hash += key[i].red_apple;
//             hash <<= 2;
//             hash += key[i].obstacle;
//         }
//         return(hash);
//     }
// };


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
    // std::array<t_q_table_key, 4>   get_t_q_table_key(const std::array<std::string, 4> &vision);


private:

    // variables
    std::ofstream   _ofs;
    std::ifstream   _ifs;
    int             _total_session;

    // functions
    void    parse_and_add_q_values(const std::string &line);
    int     parse_q_table_key(const std::string &keys_line);
    std::array<float, 4>        parse_q_table_values(const std::string &values_line);

};



#endif