#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include "argparse/include/argparse/argparse.hpp"


typedef struct s_coor
{
    int x;
    int y;
}   t_coor;

#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3

struct MyArgs : public argparse::Args {
    // std::string &anonymous = arg("an anonymous positional string argument");
    // std::string &src_path  = arg("src_path", "a positional string argument");
    // int &k                 = kwarg("k", "A keyworded integer value");
    // float &alpha           = kwarg("a,alpha", "An optional float value").set_default(0.5f);
    int &snake_size         = kwarg("s,snake_size", "Determine the initial size of the snake").set_default(3);
    int &board_size         = kwarg("b,board_size", "Determine the board size without the walls").set_default(10);
    bool &verbose           = flag("v,verbose", "A flag to toggle verbose");
};


#endif