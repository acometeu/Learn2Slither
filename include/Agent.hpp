#ifndef AGENT_HPP
#define AGENT_HPP

#include "utils.hpp"
#include "Snake.hpp"


typedef struct s_q_table_key
{
    // numbers as distance, 0 means noting
    unsigned int    green_apple;
    unsigned int    red_apple;
    unsigned int    obstacle;

    bool operator==(const s_q_table_key& key) const{
        if (obstacle == key.obstacle && green_apple == key.green_apple && red_apple == key.red_apple)
            return(true);
        return(false);
    }
} t_q_table_key;

typedef struct s_simple_q
{
    bool    green_apple;
    bool    red_apple;
    bool    obstacle;

    bool operator==(const s_simple_q& key) const{
        if (obstacle == key.obstacle && green_apple == key.green_apple && red_apple == key.red_apple)
            return(true);
        return(false);
    }
} t_simple_q;

struct hash_qtable
{
    // size_t  operator()(const std::array<s_q_table_key, 4> &key) const{
    //     return (std::hash<int>{}(key[0].obstacle + key[0].green_apple + key[0].red_apple + key[1].obstacle + key[1].green_apple + key[1].red_apple + key[2].obstacle + key[2].green_apple + key[2].red_apple + key[3].obstacle + key[3].green_apple + key[3].red_apple));
    // }


    size_t  operator()(const std::array<t_simple_q, 4> &key) const{
        size_t  hash = 0;
        int key_size = key.size();
        for (int i = 0; i < key_size; i++)
        {
            hash <<= 2;
            hash += key[i].green_apple;
            hash <<= 2;
            hash += key[i].red_apple;
            hash <<= 2;
            hash += key[i].obstacle;
        }
        return(hash);
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

    std::unordered_map< std::array<t_simple_q, 4>, std::array<float, 4>, hash_qtable>  q_table;
    // std::unordered_map< std::array<t_q_table_key, 4>, std::array<float, 4>, hash_qtable>  q_table;
    // std::unordered_map< std::array<std::string, 4>, std::array<float, 4>, hash_qtable>  q_table;

    // functions
    int     set_import_path(const std::string &import_path);
    int     set_export_path(const std::string &export_path);
    int     save_q_table_to_export_path(void);
    int     choose_direction(Snake &snake);
    int     get_safe_random_q_value(Snake &snake);
    int     get_best_q_values_direction(Snake &snake);
    void    update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir);
    std::array<t_simple_q, 4>   get_t_q_table_key(const std::array<std::string, 4> &vision);


    // q_function methods
    // static void q_funtion_classic(void);

private:

    // variables
    std::ofstream   ofs;
    std::ifstream   ifs;

    // functions
    void    parse_and_add_q_values(const std::string &line);
    std::array<t_simple_q, 4>  parse_q_table_key(const std::string &keys_line);
    std::array<float, 4>        parse_q_table_values(const std::string &values_line);

};



#endif