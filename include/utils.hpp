#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <array>
#include <deque>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <limits>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include "argparse/argparse.hpp"



// define REWARDS
#define EMPTY_REWARD -1
#define RED_APPLE_REWARD -10
#define DEATH_REWARD -100
#define GREEN_APPLE_REWARD +50
#define END_REWARD +1000
#define NO_REWARD 0

// various defines
#define DEFAULT_BAR_WIDTH 70

typedef struct s_coor
{
    int x;
    int y;

    s_coor(int y, int x) : y(y), x(x){}

    friend std::ostream& operator<<(std::ostream &cout, const s_coor &coor)
    {
        cout << coor.x << "; " << coor.y;
        return cout;
    }
}   t_coor;

typedef struct s_statistics
{
    unsigned long int turns;
    unsigned long int sessions;
    unsigned int total_length;
    unsigned int max_length;
    unsigned int consec_basic_moves;
    unsigned int stucked_in_loop;

    s_statistics() : turns(0), sessions(0), total_length(0), max_length(0), consec_basic_moves(0), stucked_in_loop(0){};
}   t_statistics;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN
}   T_direction;


typedef struct s_rgb
{
    uint8_t red, green, blue, transparancy;
}   t_rgb;


struct MyArgs : public argparse::Args {
    bool &verbose           = flag("v,verbose", "Flag to toggle verbose");
    int &sessions           = kwarg("session", "Choose the number of game sessions to play/train (default : 1)").set_default(1);
    int &snake_size         = kwarg("s,snake_size", "Snake size at start of session (default : 3)").set_default(3);
    int &board_size         = kwarg("b,board_size", "Board size of the game, in square shape (default : 10)").set_default(10);
    bool &no_learning       = flag("no_learning,no_learn", "Flag to desactivate learning, great to test/compare efficacity of specifics trained models");
    bool &no_print          = flag("no_print", "Flag to desactivate printing snake informations in console, great for fast training");
    float   &epsilon        = kwarg("e,epsilon,exploration_rate", "Agent exploration rate probability, min = 0, max = 1 (default : 0.005)").set_default(0.005f);
    float   &alpha          = kwarg("a,alpha,learning_rate", "Agent significance of new actions for training, min = 0, max = 1 (default : 0.1)").set_default(0.1f);
    float   &gamma          = kwarg("g,gamma,future_reward_weight", "Agent significance of future reward compare to instant reward for training, min = 0, max = 1 (default : 0.3)").set_default(0.3f);
    bool &visual_mode       = flag("V,view,visual", "Flag to Display graphic interface of the snake game");
    bool &step_by_step_mode = flag("step", "Flag to toggle step by step mode, for --visual mode only");
    int &snake_speed        = kwarg("snake_speed,speed", "Game latency in miliseconds, for --visual mode only (default : 500)").set_default(500);
    std::string &import_path  = kwarg("import,import_path", "Take a path to import q_table values").set_default("");
    std::string &export_path  = kwarg("export,export_path", "Take a path to export q_table values").set_default("");
    std::string &state_strategy = kwarg("strat,state_strat,stat_strategy", "choose the strategy to encode the Q_table with (default : intermediate)").set_default("intermediate");

};

void    print_vector(std::vector<char> &vector);
int     get_random_int(int min, int max);
float   get_random_float(int min, int max);
std::vector<std::string>    ft_split(std::string str, char delim);
void    print_padding(int size);
int     get_terminal_width(void);
void    loading_bar(int current, int max);




#endif