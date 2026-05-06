#include "include/utils.hpp"


void    print_vector(std::vector<char> &vector){

    for (int i = 0; i < vector.size(); i++)
        std::cout << vector[i];
}

int get_random_int(int min, int max){

    static std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<int> dist(min, max);
    return(dist(rng));
}

float get_random_float(int min, int max){

    static std::mt19937 rng(std::random_device{}());

    std::uniform_real_distribution<float> dist(min, max);
    return(dist(rng));
}
