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

std::vector<std::string>    ft_split(std::string str, char delim){

    std::vector<std::string>    table;
    size_t  pos = 0;
    size_t  end = str.find_first_of(delim);
    while (end != std::string::npos)
    {
        table.push_back(str.substr(pos, end - pos));
        pos = end + 1;
        end = str.find_first_of(delim, pos);
    }
    table.push_back(str.substr(pos));
    return (table);
}

void    print_padding(int size){

    for (int i = 0; i < size; i++)
        std::cout << ' ';
}

int get_terminal_width(){

    struct winsize window_size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size) == 0);
        return(window_size.ws_col);
    return(-1);
}

void    loading_bar(int current, int max){


    int bar_width  = get_terminal_width();
    if (bar_width == -1)
        bar_width = DEFAULT_WIDTH;

    int len = bar_width - 7;
    int progress = current * len / max;
    std::cout << "\r[";
    for (int i = 0; i < len; i++)
    {
        if (i < progress)
            std::cout << '=';
        else if (i == progress)
            std::cout << '>';
        else
            std::cout << ' ';
    }
    int pourcentage = current * 100 / max;
    std::cout << "] " << pourcentage << "%";
    std::cout.flush();
}