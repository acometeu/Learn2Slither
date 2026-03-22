#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <array>
#include <deque>
#include "argparse/argparse.hpp"


typedef struct s_coor
{
    int x;
    int y;

    friend std::ostream& operator<<(std::ostream &cout, const s_coor &coor)
    {
        cout << coor.x << "; " << coor.y;
        return cout;
    }
}   t_coor;

typedef enum
{
    RIGHT,
    UP,
    LEFT,
    DOWN
}   T_direction;


typedef struct s_rgb
{
    uint8_t red, green, blue, transparancy;
}   t_rgb;


struct MyArgs : public argparse::Args {
    // std::string &anonymous = arg("an anonymous positional string argument");
    // std::string &src_path  = arg("src_path", "a positional string argument");
    // int &k                 = kwarg("k", "A keyworded integer value");
    // float &alpha           = kwarg("a,alpha", "An optional float value").set_default(0.5f);
    int &snake_size         = kwarg("s,snake_size", "Determine the initial size of the snake").set_default(3);
    int &board_size         = kwarg("b,board_size", "Determine the board size without the walls").set_default(10);
    bool &verbose           = flag("v,verbose", "A flag to toggle verbose");
    bool &visual           = flag("V,view,visual", "A flag to toggle visual inbterface for the snake");
};



#endif