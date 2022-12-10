#include <iostream>
#include <fstream>
#include <map>

#include "penning.h"

using std::cout;
using std::endl;
#define clear_cmd system("clear");

system_configuration::system_configuration()
{
    //this->read_request_dependencies();
    this->read_saved_configurations();
    current_status = "initial_menu";

    //double 
     //       CHARGE = 1, MASS = 1, 
      //      E_C = 1, E_EPS = 0, ROT_AMPL = 0, ROT_FREQ = 0, 
       //     M_X = 0, M_Y = 0, M_Z = 10, 
        //    SIZE = 20, X = 1, Y = 0, Z = 0, VX = 0, VY = 1, VZ = 0;
    
    model_config["CHARGE"] = 1;
    model_config["MASS"] = 1;
    model_config["E_C"] = 1;
    model_config["E_EPS"] = 0;
    model_config["ROT_AMPL"] = 0;
    model_config["ROT_FREQ"] = 0;
    model_config["M_X"] = 0;
    model_config["M_Y"] = 0;
    model_config["M_Z"] = 10;
    model_config["SIZE"] = 20;
    model_config["X"] = 1;
    model_config["Y"] = 0;
    model_config["Z"] = 0;
    model_config["V_X"] = 0;
    model_config["V_Y"] = 1;
    model_config["V_Z"] = 0;
}

void execute_penning()
{
    system_configuration system_status;

    while (system_status.current_status != "exit")
    {
        system_status.print();
        system_status.get_request();
    }

    return;
}

/*void system_configuration::read_request_dependencies()
{
    std::string request_line, key_w, val_w;
    std::ifstream in("src/penning/request_dependencies.txt");

    if(!in.good())
    {
        std::cout << "Error in reading request_dependencies.txt\n";
        return;
    }

    while(in)
    {
        std::getline(in, request_line);
        if (request_line == "")
            continue;
        key_w = request_line.substr(0, request_line.find("->") - 1);
        val_w = request_line.substr(request_line.find("->") + 3, request_line.size() - request_line.find("->") - 2);
        //std::cout << key_w << " " << val_w << std::endl;
        request_dependencies.insert(std::make_pair(key_w, val_w));
    }
    in.close();

    for (std::multimap<std::string, std::string>::iterator it=request_dependencies.begin(); it!=request_dependencies.end(); ++it)
        std::cout << it->first << ' ' << it->second << std::endl;

    return;
}*/

void system_configuration::read_saved_configurations()
{
    std::string config;
    std::ifstream in("src/penning/saved_configurations.txt");
    if (!in.good())
    {
        cout << "Error in opening saved_configurations.txt\n";
        return;
    }
    while(in)
    {
        std::getline(in, config);
        configurations.push_back(config);
    }
    in.close();
}

void system_configuration::print_model()
{
    cout.precision(6);
    cout << //scientific()
            "Particle:\n" <<
            "Mass:\t" << model_config["MASS"] << endl <<
            "Charge:\t" << model_config["CHARGE"] << endl <<
            "Electrical field:\n" <<
            "Proportionality factor:\t" << model_config["E_C"] << endl <<
            "Ellipticity parameter:\t" << model_config["E_EPS"] << endl <<
            "Static:\t" << ((model_config["ROT_AMPL"] == 0 && model_config["ROT_FREQ"] == 0) ? "YES" : "NO") << endl <<
            "Rotation amplitude:\t" << model_config["ROT_AMPL"] << endl <<
            "Rotation frequency:\t" << model_config["ROT_FREQ"] << endl <<
            "Magnetic field:\n" <<
            "(" << model_config["M_X"] << ", " << model_config["M_Y"] << ", " << model_config["M_Z"] << ")\n" <<
            "Geomtry:\n" <<
            "Detector size:\t" << model_config["SIZE"] << endl <<
            "Particle posotion:\t" << 
            "(" << model_config["X"] << ", " << model_config["Y"] << ", " << model_config["Z"] << ")\n" <<
            "Particle velocity" <<
            "(" << model_config["V_X"] << ", " << model_config["V_Y"] << ", " << model_config["V_Z"] << ")\n\n";
    return;
}

void system_configuration::print()
{
    if (clear)
        clear_cmd;
    if (incorrect_input)
    {
        cout << "Incorret input, please, try again\n\n";
    }
    else if (current_status == "initial_menu")
    {
        cout << 
            "initial menu\n" <<
            "1) Main menu\n" <<
            "2) Exit\n";
    }
    else if (current_status == "main_menu")
    {
        std::string config_line;
        std::ifstream in;

        cout << "Choose configuration:\n\n";

        for (int i = 0; i < configurations.size(); ++i)
        {
            cout << i + 1 << ") " << configurations[i] << endl;
        }
        cout << configurations.size() + 1 << ") new configuration ...\n" <<
                configurations.size() + 2 << ") exit\n";
    }
    else if (current_status == "new_config")
    {
        cout << "Variables:\n\n" << endl;
        print_model();
        cout << "Change:\n" <<
                "1) Particle\n" <<
                "2) Electric field\n" <<
                "3) Magnetic field\n" <<
                "4) Geometry\n" <<
                "5) Save\n" <<
                "6) Exit\n";
    }
    else if (current_status == "new_config->particle")
    {
        cout << "Write:\n" << 
                "mass <value>\n" <<
                "charge <value>\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->E_field")
    {
        cout << "Write:\n" << 
                "static\n" <<
                "rotating\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->E_field->static")
    {
        cout << "Write:\n" << 
                "prop <value>\n" <<
                "ellipse <value>\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->E_field->rotating")
    {
        cout << "Write:\n" << 
                "prop <value>\n" <<
                "ellips <value>\n" <<
                "ampl <value>\n" <<
                "freq <value>\n" <<
                "exit\n";
    }
    else if (current_status == "new_cofing->M_field")
    {
        cout << "Write:\n" << 
                "cartesian\n" <<
                "spherical\n" <<
                "exit\n";
    }
    else if (current_status == "new_cofing->M_field->cartesian")
    {
        cout << "Write:\n" << 
                "<M_x> <M_y> <M_z>\n" <<
                "exit\n";
    }
    else if (current_status == "new_cofing->M_field->spherical")
    {
        cout << "Write:\n" << 
                "<M_r> <M_theta> <M_phi>\n" <<
                "(To radians write -r at the end of the line.\n" <<
                "Also, you can use 'pi')\n"
                "exit\n";
    }
    else if (current_status == "new_cofing->geometry")
    {
        cout << "Write:\n" << 
                "size\n" <<
                "coord\n" <<
                "velocity\n" <<
                "exit\n";
    }
    else if (current_status == "new_cofing->geometry->size")
    {
        cout << "Write:\n" << 
                "<value>\n" <<
                "exit\n";
    }
    else if (current_status == "new_cofing->geometry->coordinate")
    {
        cout << "Write:\n" << 
                "<X> <Y> <Z>\n" <<
                "exit\n";
    }
    else if (current_status == "new_cofing->geometry->velocity")
    {
        cout << "Write:\n" << 
                "<V_x> <V_y> <V_z>\n" <<
                "exit\n";
    }
    else if (current_status == "new_cofing->save")
    {
        cout << "Write the name or press Enter to create default name\n";
    }
    else if (current_status == "new_cofing->exit")
    {
        cout << "Are you sure you want to exit and reset configuration? [y/n]\n";
    }
    else if (current_status == "pre-launch_window")
    {
        cout << "Configuration: " << model_name << endl;
        print_model();
        cout << "Write:\n" <<
                "start\n" <<
                "exit\n";
    }
    else if (current_status == "model")
    {
        cout << "something about status maybe and warning if particle flew out of box\n";

        cout << "write:\n" <<
                "3d plot\n" <<
                "projection <two coordinates>\n" <<
                "time <one coordinate>\n" <<
                "exit\n";
    }
    cout << "penning> ";
    return;
}

void system_configuration::get_request()
{
    std::string income_command;

    std::cin >> income_command;

    if (income_command == "")
        return;

}