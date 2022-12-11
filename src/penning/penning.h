#ifndef PENNING_H
#define PENNING_H

#include <fstream>
#include <string>
#include <map>
#include <vector>

void execute_penning();

class system_configuration final
{
private:
    std::string current_status;

    std::map<std::string, double> model_config;
    //std::multimap<std::string, std::string> request_dependencies;
    std::vector<std::string> configurations;

    bool clear = true, incorrect_input = false, print_ = true;
    std::string income_command, model_name = "unknown";

    system_configuration();

    void read_request_dependencies();
    void read_saved_configurations();
    void read_configuration(std::string);
    void reset_config();
    void print_model();
    void save_model(std::ofstream&);
    void print();
    void get_request();
    void stop();
    
    friend void execute_penning();
};

#endif // PENNING_H