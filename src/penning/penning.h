#ifndef PENNING_H
#define PENNING_H

#include <string>
#include <map>
#include <vector>

class system_configuration final
{
private:
    std::string current_status;

    std::map<std::string, double> model_config;
    //std::multimap<std::string, std::string> request_dependencies;
    std::vector<std::string> configurations;

    bool clear, incorrect_input;
    std::string model_name = "unknown";

    system_configuration();

    void read_request_dependencies();
    void read_saved_configurations();
    void print_model();
    void print();
    void get_request();

    friend void execute_penning();
};

#endif // PENNING_H