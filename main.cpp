#include "include/visuals.hpp"


int game_loop(Board &board, Snake &snake, MyArgs &args, Agent &agent){

    for (int i = 1; i <= args.sessions; i++)
    {
        if (args.no_print)
            loading_bar(i, args.sessions);
        bool    running = true;
        while(running)
        {
            snake.dir = agent.choose_direction(snake, args, i);
            if (snake.update_position_and_q_values(args, agent))
                break;
        }
        snake.stats_add_session();
        if (i < args.sessions)
        {
            board.reset();
            snake.reset();
        }
    }

    snake.display_stats();
    if (!args.export_path.empty())
    {
        if (agent.save_q_table_to_export_path())
            return (1);
    }
    return(0);
}

AStateStrategy  *choose_strategy(MyArgs &args){

    if (args.state_strategy == "simple" || args.state_strategy == "simple_strategy")
        return (new SimpleStateStrategy());
    else if (args.state_strategy == "" || args.state_strategy == "intermediate" || args.state_strategy == "intermediate_strategy")
        return(new IntermediateStateStrategy());
    else if (args.state_strategy == "complex" || args.state_strategy == "complex_strategy")
        return(new ComplexStateStrategy());
    else
        return (new IntermediateStateStrategy());
}

void    print_strategy(MyArgs &args, Agent &agent){

    if (args.verbose)
    {
        if (dynamic_cast<SimpleStateStrategy*>(agent.strategy) != nullptr)
            std::cout << "--strat selected : Simple strategy (fast learning, lowest space, worse results)" << std::endl;
        else if (dynamic_cast<IntermediateStateStrategy*>(agent.strategy) != nullptr)
            std::cout << "--strat selected : Intermediate strategy (medium speed learning, medium space, best results)" << std::endl;
        else if (dynamic_cast<ComplexStateStrategy*>(agent.strategy) != nullptr)
            std::cout << "--strat selected : Complex strategy (slow learning, huge space, medium results, dependant of training board size used)" << std::endl;
        else
            std::cout << "--strat selected : Not recognised" << std::endl;
    }

    return;
}

int learn2slither(Board &board, Snake &snake, MyArgs &args){

    AStateStrategy *strat = choose_strategy(args);
    Agent   agent(args.epsilon, args.alpha, args.gamma, args.sessions, strat);

    if (!args.import_path.empty())
    {
        if (agent.set_import_path(args.import_path))
            return (1);
    }

    if (!args.export_path.empty())
    {
        if (agent.set_export_path(args.export_path))
            return (1);
    }

    print_strategy(args, agent);
    board.print_board();
    snake.update_vision();
    if (!args.no_print)
        snake.print_vision();

    if (args.visual_mode)
    {
        if (game_loop_SDL(board, snake, args, agent))
            return (1);
    }
    else
    {
        if (game_loop(board, snake, args, agent))
            return(1);
    }

    return (0);
}

#include "include/SimpleStateStrategy.hpp"
int main(int argc, char* argv[]) {
    MyArgs args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    Board board(args.board_size);
    Snake snake(board, args.snake_size);

    if (learn2slither(board, snake, args))
        return (1);

    return 0;
}
