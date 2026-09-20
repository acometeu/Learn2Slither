#ifndef DQN_HPP
#define DQN_HPP

#include "AQMethod.hpp"


#define OUTPUT_NBR 4

class DQN : public AQMethod
{
public:
    //variables
    std::vector<Eigen::MatrixXf>   dqn_weights;
    std::vector<Eigen::MatrixXf>   dqn_bias;


    // functions
    DQN(float alpha, float gamma, AStateStrategy *state);
    ~DQN();
    virtual int     set_q_values(std::ifstream &ifs) override;
    virtual int     save_q_values(std::ofstream &ofs) const override;
    virtual int     get_best_q_values_direction(Snake &snake) const override;
    virtual void    update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir) override;    


private:
    // variables
    int _hidden_layer_nbr;
    int _node_per_hidden_layer;
    int _first_layer_node_number;

    // functions
    void    initialize_neural_network(void);
    void    initialize_first_layer(void);
    void    initialize_hidden_layers(void);
    void    initialize_last_layer(void);
    int     set_q_values_params(const std::string &line);
    int     set_q_values_first_layer(std::ifstream &ifs, std::string &line, std::vector<Eigen::MatrixXf> &dqn);
    int     set_q_values_hidden_layers(std::ifstream &ifs, std::string &line, std::vector<Eigen::MatrixXf> &dqn);
    int     set_q_values_last_layer(std::ifstream &ifs, std::string &line, std::vector<Eigen::MatrixXf> &dqn);
    std::array<float, 4>        parse_q_table_values(const std::string &values_line);
    // const std::array<int, 4>    get_dqn_output(const std::vector<int> key) const;



    // debug
    void    print_dqn_weights(void);
    void    print_dqn_bias(void);

};



#endif