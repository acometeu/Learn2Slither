#include "include/utils.hpp"


void    print_vector(std::vector<char> &vector){

    for (int i = 0; i < vector.size(); i++)
        std::cout << vector[i];
}