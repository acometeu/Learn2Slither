#ifndef DQN_HPP
#define DQN_HPP

#include "AQMethod.hpp"


#define OUTPUT_NBR 4

class DQN : public AQMethod
{
public:
    //variables
    std::vector< std::vector< std::vector<float> > >   dqn_weights;
    std::vector< std::vector< std::vector<float> > >   dqn_bias;


    // functions
    DQN(float alpha, float gamma, AStateStrategy *state);
    ~DQN();
    virtual int     set_q_values(std::ifstream &ifs) override;
    virtual int     save_q_values(std::ofstream &ofs) const override;
    virtual int     get_best_q_values_direction(Snake &snake) const override;
    virtual void    update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir) override;    


private:
    // variables
    int hidden_layer_nbr;
    int node_per_hidden_layer;

    // functions
    void    initialize_neural_network(void);
    std::array<float, 4>        parse_q_table_values(const std::string &values_line);


    // debug
    void    print_dqn(void);

};



#endif