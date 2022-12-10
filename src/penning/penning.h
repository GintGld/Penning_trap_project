#ifndef PENNING_H
#define PENNING_H

#include <string>

int sum(int, int);

class system_configuration final
{
private:
    std::string income_command;
    std::string current_status;

public:

    bool is_command_exit(std::string);

    std::string init();
    std::string menu();
    std::string new_config();

};


#endif // PENNING_H