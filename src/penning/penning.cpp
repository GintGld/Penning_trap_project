#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

#include "penning.h"
#include "../phys/phys.h"

using std::cin;
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
    system_status.stop();
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

void system_configuration::save_model(std::ofstream& out)
{
    out <<
        "CHARGE : "   << model_config["CHARGE"] << endl << 
        "MASS : "     << model_config["MASS"] << endl << 
        "E_C : "      << model_config["E_C"] << endl << 
        "E_EPS : "    << model_config["E_EPS"] << endl << 
        "ROT_AMPL : " << model_config["ROT_AMPL"] << endl << 
        "ROT_FREQ : " << model_config["ROT_FREQ"] << endl << 
        "M_X : "      << model_config["M_X"] << endl << 
        "M_Y : "      << model_config["M_Y"] << endl << 
        "M_Z : "      << model_config["M_Z"] << endl << 
        "SIZE : "     << model_config["SIZE"] << endl << 
        "X : "        << model_config["X"] << endl << 
        "Y : "        << model_config["Y"] << endl << 
        "Z : "        << model_config["Z"] << endl << 
        "V_X : "      << model_config["V_X"] << endl << 
        "V_Y : "      << model_config["V_Y"] << endl << 
        "V_Z : "      << model_config["V_Z"];
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
            "Write\n" <<
            "start\n" <<
            "exit\n";
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
                "prop <value> (must be > 0)\n" <<
                "ellipse <value> (lies in [-1,1])\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->E_field->rotating")
    {
        cout << "Write:\n" << 
                "prop <value> (must be > 0)\n" <<
                "ellips <value> (lies in [-1,1])\n" <<
                "ampl <value>\n (must be > 0)" <<
                "freq <value> (must be > 0)\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->M_field")
    {
        cout << "Write:\n" << 
                "cartesian\n" <<
                "spherical\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->M_field->cartesian")
    {
        cout << "Write:\n" << 
                "<M_x> <M_y> <M_z>\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->M_field->spherical")
    {
        cout << "Write:\n" << 
                "<M_r> <M_theta> <M_phi>\n" <<
                "(angles are in degrees)"
                "exit\n";
    }
    else if (current_status == "new_config->geometry")
    {
        cout << "Write:\n" << 
                "size\n" <<
                "coord\n" <<
                "vel\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->geometry->size")
    {
        cout << "Write:\n" << 
                "<value>\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->geometry->coordinate")
    {
        cout << "Write:\n" << 
                "<X> <Y> <Z>\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->geometry->velocity")
    {
        cout << "Write:\n" << 
                "<V_x> <V_y> <V_z>\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->save")
    {
        cout << "Write the name\n";
    }
    else if (current_status == "new_config->exit")
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
    cin >> income_command;
    cout << endl;

    incorrect_input = false;
    clear = true;

    {
        int last = income_command.find_last_not_of(' ') == std::string::npos ? 0 : income_command.find_last_not_of(' ');
        income_command = income_command.substr(0, last);
        int first = income_command.find_first_not_of(' ') == std::string::npos ? 0 : income_command.find_first_not_of(' ');
        income_command = income_command.substr(first, income_command.size() - first + 1);
    }
    if (income_command == "")
    {
        clear = false;
        return;
    }
    else if (income_command == "initial_menu")
    {
        if (income_command == "start")
        {
            current_status = "main_menu";
        }
        else if (income_command == "exit")
        {
            current_status = "exit";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
        }
    }
    else if (income_command == "main_menu")
    {
        int i = -1;
        if (income_command == std::to_string(configurations.size() + 2) || income_command == "Exit" || income_command == "exit")
        {
            current_status = "initial_menu";
            return;
        }
        try{
            i = std::stoi(income_command);
            if (i >= 1 && i <= configurations.size())
            {
                model_name = configurations[i - 1];
                current_status = "pre-launch_window";
                return;
            }
            else if (i == configurations.size() + 1)
            {
                current_status = "new_config";
                return;
            }
            else if (i == configurations.size() + 2)
            {
                current_status = "initial_menu";
                return;
            }
            else
            {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
        catch (...) {
            clear = false;
            incorrect_input = true;
            return;
        }
        clear = false;
        incorrect_input = true;
        return;
    }
    else if (income_command == "new_config")
    {
        if (income_command == "1" || income_command == "Particle" || income_command == "particle")
        {
            current_status = "new_config->particle";
            return;
        }
        else if (income_command == "2" || income_command == "Electric" || income_command == "electric" || income_command == "Electric field" || income_command == "electric field")
        {
            current_status = "new_config->E_field";
            return;
        }
        else if (income_command == "3" || income_command == "Magnetic" || income_command == "magnetic" || income_command == "Magnetic field" || income_command == "magnetic field")
        {
            current_status = "new_config->M_field";
            return;
        }
        else if (income_command == "4" || income_command == "Geometry" || income_command == "geometry")
        {
            current_status = "new_config->geometry";
            return;
        } 
        else if (income_command == "5" || income_command == "Save" || income_command == "save")
        {
            current_status = "new_config->save";
            return;
        }
        else if (income_command == "6" || income_command == "Exit" || income_command == "exit")
        {
            current_status = "new_config->exit";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (income_command == "new_config->particle")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        double par = 0;
        try {
            int i = income_command.find(' ');
            par = std::stod(income_command.substr(i+1, income_command.size() - i));
            if (par == 0)
                throw "parameter can't be 0";
        }
        catch (...) {
            clear = false;
            incorrect_input = true;
            return;
        }
        if (income_command.substr(0,5) == "mass " && par > 0)
        {
            model_config["MASS"] = par;
            current_status = "new_config";
            return;
        }
        else if (income_command.substr(0,7) == "charge ")
        {
            model_config["CHARGE"] = par;
            current_status = "new_config";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (income_command == "new_config->E_field")
    {
        if (income_command == "static" || income_command == "Static")
        {
            current_status = "new_config->E_field->static";
            return;
        }
        else if (income_command == "rotating" || income_command == "Rotating")
        {
            current_status = "new_config->E_field->rotating";
            return;
        }
        else if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (income_command == "new_config->E_field->static")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        double par = 0;
        try {
            int i = income_command.find(' ');
            par = std::stod(income_command.substr(i+1, income_command.size() - i));
        }
        catch (...) {
            clear = false;
            incorrect_input = true;
            return;
        }
        if (income_command.substr(0,5) == "prop " && par > 0)
        {
            model_config["E_C"] = par;
            current_status = "new_config";
            return;
        }
        else if (income_command.substr(0,8) == "ellipse " && par >= -1 && par <= 1)
        {
            model_config["E_EPS"] = par;
            current_status = "new_config";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (income_command == "new_config->E_field->rotating")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        double par = 0;
        try {
            int i = income_command.find(' ');
            par = std::stod(income_command.substr(i+1, income_command.size() - i));
        }
        catch (...) {
            clear = false;
            incorrect_input = true;
            return;
        }
        if (income_command.substr(0,5) == "prop " && par != 0)
        {
            model_config["E_C"] = par;
            current_status = "new_config";
            return;
        }
        else if (income_command.substr(0,8) == "ellipse " && par >= -1 && par <= 1)
        {
            model_config["E_EPS"] = par;
            current_status = "new_config";
            return;
        }
        else if (income_command.substr(0,5) == "ampl " && par >= 0)
        {
            model_config["ROT_AMPL"] = par;
            current_status = "new_config";
            return;
        }
        else if (income_command.substr(0,5) == "freq " && par >= 0)
        {
            model_config["ROT_FREQ"] = par;
            current_status = "new_config";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (income_command == "new_config->M_field")
    {
        if (income_command == "cartesian")
        {
            current_status = "new_config->M_field->cartesian";
            return;
        }
        else if (income_command == "spherical")
        {
            current_status = "new_config->M_field->spherical";
            return;
        }
        else if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (income_command == "new_config->M_field->cartesian")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            try {
                double x, y, z;
                std::string s1, s2, s3;
                int i = 0;
                while (income_command[i] != ' ' && i < income_command.size())
                    ++i;
                --i;
                int j = i + 2;
                while (income_command[j] != ' ' && j < income_command.size())
                    ++j;
                --j;
                s1 = income_command.substr(0, i + 1);
                s2 = income_command.substr(i + 2, j - i - 1);
                s3 = income_command.substr(j + 2, income_command.size() - j - 1);
                model_config["M_X"] = std::stod(s1);
                model_config["M_Y"] = std::stod(s2);
                model_config["M_Z"] = std::stod(s3);
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
    }
    else if (income_command == "new_config->M_field->spherical")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            try {
                double r, theta, phi;
                std::string s1, s2, s3;
                int i = 0;
                while (income_command[i] != ' ' && i < income_command.size())
                    ++i;
                --i;
                int j = i + 2;
                while (income_command[j] != ' ' && j < income_command.size())
                    ++j;
                --j;
                s1 = income_command.substr(0, i + 1);
                s2 = income_command.substr(i + 2, j - i - 1);
                s3 = income_command.substr(j + 2, income_command.size() - j - 1);
                r     = std::stod(s1);
                theta = std::stod(s2);
                phi   = std::stod(s3);
                model_config["M_X"] = r * sin(theta) * cos(phi);
                model_config["M_Y"] = r * sin(theta) * sin(phi);
                model_config["M_Z"] = r * cos(theta);
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
    }
    else if (income_command == "new_config->geometry")
    {
        if (income_command == "size" || income_command == "Size")
        {
            current_status = "new_config->geometry->size";
            return;
        }
        else if (income_command == "coord" || income_command == "Coord" || income_command == "coordinate" || income_command == "Coordinate")
        {
            current_status = "new_config->geometry->coordinate";
            return;
        }
        else if (income_command == "velocity" || income_command == "Velovity" || income_command == "vel" || income_command == "Vel")
        {
            current_status = "new_config->geometry->velocity";
            return;
        }
        else if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (income_command == "new_config->geometry->size")
    {
        double par = -1;
        try {
            par = std::stod(income_command);
        }
        catch (...) {
            clear = false;
            incorrect_input = true;
            return;
        }
        if (par > 0)
        {
            model_config["SIZE"] = par;
            current_status = "new_config";
            return;
        }
    }
    else if (income_command == "new_config->geometry->coordinate")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            try {
                double x, y, z;
                std::string s1, s2, s3;
                int i = 0;
                while (income_command[i] != ' ' && i < income_command.size())
                    ++i;
                --i;
                int j = i + 2;
                while (income_command[j] != ' ' && j < income_command.size())
                    ++j;
                --j;
                s1 = income_command.substr(0, i + 1);
                s2 = income_command.substr(i + 2, j - i - 1);
                s3 = income_command.substr(j + 2, income_command.size() - j - 1);
                model_config["X"] = std::stod(s1);
                model_config["Y"] = std::stod(s2);
                model_config["Z"] = std::stod(s3);
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
    }
    else if (income_command == "new_config->geometry->velocity")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            try {
                double x, y, z;
                std::string s1, s2, s3;
                int i = 0;
                while (income_command[i] != ' ' && i < income_command.size())
                    ++i;
                --i;
                int j = i + 2;
                while (income_command[j] != ' ' && j < income_command.size())
                    ++j;
                --j;
                s1 = income_command.substr(0, i + 1);
                s2 = income_command.substr(i + 2, j - i - 1);
                s3 = income_command.substr(j + 2, income_command.size() - j - 1);
                model_config["V_X"] = std::stod(s1);
                model_config["V_Y"] = std::stod(s2);
                model_config["V_Z"] = std::stod(s3);
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
    }
    else if (income_command == "new_config->save")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        std::ofstream out("saved_configurations/"+income_command+".txt");
        if (!out.good())
        {
            out.close();
            clear = false;
            incorrect_input = true;
            return;
        }
        save_model(out);
        out.close();
        configurations.push_back(income_command);
        current_status = "main_menu";
        return;
    }
    else if (income_command == "new_config->exit")
    {
        if (income_command == "y" || income_command == "yes" || income_command == "Y" || income_command == "Yes")
        {
            current_status = "main_menu";
            return;
        }
        else if (income_command == "n" || income_command == "no" || income_command == "N" || income_command == "No")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else
    {
        cout << "Unknown status. stopping program\n";
        current_status = "exit";
        clear = false;
        return;
    }
}

void system_configuration::stop()
{
    std::ofstream out("saved_configurations.txt");
    if (!out.good())
    {
        out.close();
        cout << "Cannot open saved_configurations.txt.\nFATAL ERROR\n";
        return;
    }
    for (int i = 0; i < configurations.size(); ++i)
    {
        out << configurations[i] << endl;
    }
    out.close();

    try {
        system("ls > src/penning/ls_list.txt");
    }
    catch (...) {
        cout << "Cannot run 'ls > src/penning/ls_list.txt'\nFATA ERROR\n";
        return
    }

    std::ifstream in("src/penning/ls_list.txt");
    if (!in.good())
    {
        cout << "Cannot open ls_list.txt\nFATAL ERROR\n";
        return;
    }

    std::string file;
    bool found;
    while(in)
    {
        found = false;
        std::getline(in, file);
        for (int i = 0; i < configuration.size(); ++i)
            found &= (file == configuration[i]);
        if (!found)
        {
            try {
                system("rm src/penning/"+file+".txt");
            }
            catch (...) {
                cout << "Cannot run 'rm src/penning/" << file << ".txt'\nFATAL ERROR\n";
                return;
            }
        }
    }

    try {
        system("rm src/penning/ls_list.txt")
    }
    catch (...) {
        cout << "Cannot run 'rm src/penning/ls_list.txt'\nFATA ERROR\n";
        return
    }
    return;
}