#ifndef PENNING_H
#define PENNING_H

#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "phys/phys.h"

void execute_penning();

class system_configuration final
{
private:
    std::map<std::string, double> model_config;
    std::vector<std::string> configurations;

    bool clear = true, incorrect_input = false, print_ = true, is_out_of_borders = false;
    std::string current_status, income_command, model_name = "undefined";
    double time = 0, time_step, n_repeat;
    space<double> model_space = new_space<double>();
    std::vector<double> x, y, z, vx, vy, vz, t;

    system_configuration();

    void count(double);
    void read_request_dependencies();
    void read_saved_configurations();
    void read_configuration(std::string);
    void create_binary_dir();
    void delete_binary_dir();
    void reset_config();
    bool is_config_changed();
    void print_model();
    void save_model(std::ofstream&);
    void print();
    void get_request();
    void stop();
    
    friend void execute_penning();
};

#endif // PENNING_H