#include "include/Snake.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


// #include <SFML/Graphics.hpp>
// int run_SFML(void){
//     sf::RenderWindow window(sf::VideoMode({200, 200}), "SFML works!");
//     sf::CircleShape shape(100.f);
//     shape.setFillColor(sf::Color::Green);

//     while (window.isOpen())
//     {
//         while (const std::optional event = window.pollEvent())
//         {
//             if (event->is<sf::Event::Closed>())
//                 window.close();
//         }

//         window.clear();
//         window.draw(shape);
//         window.display();
//     }
//     return(0);
// }


int run_SDL(void){
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        return(1);
    }

    SDL_Quit();
    return(0);
}


int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    Board board(args.board_size);
    Snake snake(board, args.snake_size);
    board.print_board();

    if (args.visual)
    {
        if (run_SDL())
            return (1);
    }

    return 0;
}
