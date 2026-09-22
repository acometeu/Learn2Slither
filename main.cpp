#include "include/visuals.hpp"


int game_loop(Board &board, Snake &snake, MyArgs &args, Agent &agent){

    for (int i = 1; i <= args.sessions; i++)
    {
        if (args.no_print)
            loading_bar(i, args.sessions);
        else
        {
            std::cout << "session " << i << std::endl << std::endl;
            snake.print_vision();
        }

        bool    running = true;
        while(running)
        {
            snake.dir = agent.choose_direction(snake, args, i);
            if (snake.update(args, agent))
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
    if (agent.save_q_table_to_export_path(args))
        return (1);
    return(0);
}

AStateStrategy  *choose_strategy(MyArgs &args){

    // if (args.state_strategy == "simple" || args.state_strategy == "simple_strategy")
    //     return (new SimpleStateStrategy());
    // else if (args.state_strategy == "" || args.state_strategy == "intermediate" || args.state_strategy == "intermediate_strategy")
        return(new IntermediateStateStrategy());
    // else if (args.state_strategy == "complex" || args.state_strategy == "complex_strategy")
    //     return(new ComplexStateStrategy());
    // else
    //     return (new IntermediateStateStrategy());
}

void    print_strategy(MyArgs &args, AStateStrategy *state){

    if (args.verbose)
    {
        if (dynamic_cast<SimpleStateStrategy*>(state) != nullptr)
            std::cout << "--strat selected : Simple strategy (fast learning, lowest space, worse results)" << std::endl;
        else if (dynamic_cast<IntermediateStateStrategy*>(state) != nullptr)
            std::cout << "--strat selected : Intermediate strategy (medium speed learning, medium space, best results)" << std::endl;
        else if (dynamic_cast<ComplexStateStrategy*>(state) != nullptr)
            std::cout << "--strat selected : Complex strategy (slow learning, huge space, medium results, dependant of training board size used)" << std::endl;
        else
            std::cout << "--strat selected : Not recognised" << std::endl;
    }

    return;
}

int learn2slither(Board &board, Snake &snake, MyArgs &args){

    AStateStrategy *state = choose_strategy(args);
    //testtemp ajouteer un vrai parsing pour avoir la methode souhaitee (Q_table ou DQN)
    // AQMethod *method = new QTable(args.alpha, args.gamma, state);
    
    //testsuppr
    AQMethod *method = new DQN(args.alpha, args.gamma, state);
    // delete method;
    // Eigen::MatrixXf test1_8(1, 8);
    // test1_8 << 1, 2, 3, 4, 5, 6, 7, 8;
    // std::cout << "test1_8 : " << std::endl;
    // std::cout << test1_8 << std::endl; 
    // Eigen::MatrixXf test8_4(8, 4);
    // test8_4.row(0).setConstant(1);
    // test8_4.row(1).setConstant(2);
    // test8_4.row(2).setConstant(3);
    // test8_4.row(3).setConstant(4);
    // test8_4.row(4).setConstant(1);
    // test8_4.row(5).setConstant(2);
    // test8_4.row(6).setConstant(3);
    // test8_4.row(7).setConstant(4);
    // std::cout << "test8_4 : " << std::endl;
    // std::cout << test8_4 << std::endl;
    
    // Eigen::MatrixXf test = test1_8 * test8_4;
    // std::cout << "test : " << std::endl;
    // std::cout << test << std::endl;
    // return(0);


    //testsuppr
    if (method == nullptr)
    {
        std::cout << "method == nullptr" << std::endl;
        return(1);
    }
    if (method == NULL)
    {
        std::cout << "method == NULL" << std::endl;
        return(1);
    }
    // if (dynamic_cast<QTable*>(method) == nullptr)
    // {
    //     std::cout << "method not QTable" << std::endl;
    //     return(1);
    // }

    Agent   agent(args.epsilon, args.alpha, args.gamma, args.sessions, method, state);

    if (!args.import_path.empty())
    {
        if (agent.set_import_path(args.import_path))
            return (1);
    }

    //testsuppr
    std::cout << "bestdir = " << agent.q_method->get_best_q_values_direction(snake) << std::endl;
    // if (dynamic_cast<QTable*>(agent.q_method) == nullptr)
    // {
    //     std::cout << "agent.q_method not QTable" << std::endl;
    //     return(1);
    // }


    if (!args.export_path.empty())
    {
        if (agent.set_export_path(args.export_path))
            return (1);
    }

    print_strategy(args, state);
    snake.update_vision();
    board.print_board();

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
