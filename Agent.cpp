#include "include/Agent.hpp"

Agent::Agent(float epsilon, float alpha, float gamma, int total_session, AQMethod *q_method, AStateStrategy *state): epsilon(epsilon), _total_session(total_session), q_method(q_method){

    return;
}

Agent::~Agent(){

    if (_ofs.is_open())
        _ofs.close();
    if (_ifs.is_open())
        _ifs.close();
    delete q_method;
    return;
}

int     Agent::set_import_path(const std::string &import_path){

    _ifs.open(import_path);
    if (!_ifs.is_open())
    {
        std::cerr << "Failed opening import path : " << import_path << std::endl;
        return(1);
    }

    q_method->set_q_values(_ifs);
    return(0);
}

int     Agent::set_export_path(const std::string &export_path){

    _ofs.open(export_path, std::ofstream::trunc);
    if (!_ofs.is_open())
    {
        std::cerr << "Failed opening export path : " << export_path << std::endl;
        return(1);
    }
    return(0);
}

int     Agent::save_q_table_to_export_path(MyArgs &args){

    if (args.export_path.empty())
        return(0);

    if (!_ofs.is_open())
    {
        std::cerr << "Saved to export file failed : ofs stream closed" << std::endl;
        return (1);
    }

    if (q_method->save_q_values(_ofs))
        return(1);

    return(0);
}

int    Agent::choose_direction(Snake &snake, MyArgs &args, int current_session){
    
    // exploration rate
    if (!args.no_learning)
    {
        if (current_session <= _total_session * 0.25) //~first quartile
        {
            if (get_random_float(0, 1) < epsilon)
                return(get_random_int(0, 3));
        }
    }

    return(q_method->get_best_q_values_direction(snake));
}


void    Agent::update_q_value(Snake &snake, int reward, const std::array<std::string, 4> &old_state, int old_dir){

    q_method->update_q_value(snake, reward, old_state, old_dir);
}