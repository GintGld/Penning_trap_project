#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

#include "penning.h"
#include "phys/phys.h"
#include "matplot/matplot.h"

using std::cin;
using std::cout;
using std::endl;
#define clear_cmd system("clear");

system_configuration::system_configuration()
{
    //this->read_request_dependencies();
    current_status = "initial_menu";
    this->read_saved_configurations();

    //double 
     //       CHARGE = 1, MASS = 1, 
      //      E_C = 1, E_EPS = 0, ROT_AMPL = 0, ROT_FREQ = 0, 
       //     M_X = 0, M_Y = 0, M_Z = 10, 
        //    SIZE = 20, X = 1, Y = 0, Z = 0, VX = 0, VY = 1, VZ = 0;
    
    reset_config();
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
        if (config != "")
            configurations.push_back(config);
    }
    in.close();
}

void system_configuration::read_configuration(std::string file)
{
    std::ifstream in("src/penning/saved_configurations/"+file+".txt");
    if (!in.good())
    {
        in.close();
        cout << "Can't open " << file << ".txt" << endl << "FATAL ERROR\n";
        return;
    }
    std::string line;
    try {
        while(in)
        {
            std::getline(in, line);
            std::string s1 = line.substr(0, line.find(" : "));
            std::string s2 = line.substr(line.find(" : ") + 3, line.size() - line.find(" : ") - 2);
            model_config[s1] = std::stod(s2);
        }
        in.close();
        return;
    }
    catch (...) {
        in.close();
        cout << "Can't open " << file << ".txt" << endl << "FATAL ERROR\n";
        return;
    }
    in.close();
    return;
}

void system_configuration::reset_config()
{
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
    return;
}

void system_configuration::print_model()
{
    cout.precision(3);
    cout << "----------------------------------" << endl <<
            "Particle\n" <<
            "Mass:\t\t\t" << model_config["MASS"] << endl <<
            "Charge:\t\t\t" << model_config["CHARGE"] << endl <<
            "----------------------------------" << endl << 
            "Electrical field\n" <<
            "Proportionality factor:\t" << model_config["E_C"] << endl <<
            "Ellipticity parameter:\t" << model_config["E_EPS"] << endl <<
            "Static:\t\t\t" << ((model_config["ROT_AMPL"] == 0 && model_config["ROT_FREQ"] == 0) ? "YES" : "NO") << endl <<
            "Rotation amplitude:\t" << model_config["ROT_AMPL"] << endl <<
            "Rotation frequency:\t" << model_config["ROT_FREQ"] << endl <<
            "----------------------------------" << endl << 
            "Magnetic field\t\t" <<
            "(" << model_config["M_X"] << ", " << model_config["M_Y"] << ", " << model_config["M_Z"] << ")\n" <<
            "----------------------------------" << endl << 
            "Geometry\n" <<
            "Detector size:\t\t" << model_config["SIZE"] << endl <<
            "Particle posotion:\t" << 
            "(" << model_config["X"] << ", " << model_config["Y"] << ", " << model_config["Z"] << ")\n" <<
            "Particle velocity:\t" <<
            "(" << model_config["V_X"] << ", " << model_config["V_Y"] << ", " << model_config["V_Z"] << ")\n" <<
            "----------------------------------" << endl;
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

void system_configuration::count(double time)
{
    model_space.set_particle( new_particle<double>(
        vector3d<double> (model_config["X"], model_config["Y"], model_config["Z"]),
        vector3d<double> (model_config["V_X"], model_config["V_Y"], model_config["V_Z"]),
        model_config["CHARGE"], model_config["MASS"]));
    model_space.set_E_field(penning_E(model_config["E_C"], model_config["E_EPS"]));
    model_space.set_M_field( penning_M(
        vector3d<double>(model_config["M_X"], model_config["M_Y"], model_config["M_Z"])));

    model_space.solve(time, .01, "rk45", 10);
    model_space.write("src/penning/output");

    vector3d<double> r;
    x.clear(); y.clear(); z.clear();
    vx.clear(); vy.clear(); vz.clear();
    t.clear();

    std::ifstream in("src/penning/output.binary", std::ios::binary);
    for (int i = 0; i < (unsigned)(time / .01); ++i)
    {
        in.read((char*) &r, sizeof(vector_d));
        x.push_back(r.x());
        y.push_back(r.y());
        z.push_back(r.z());
        in.read((char*) &r, sizeof(vector_d));
        vx.push_back(r.x());
        vy.push_back(r.y());
        vz.push_back(r.z());
        t.push_back((double)(.01 * i));
    }
    in.close();
    try {
        system("rm src/penning/output.binary");
    }
    catch (...) {
        cout << "Cannot run rm src/penning/output.binary";
        return;
    }
}

void system_configuration::print()
{
    if (!print_)
    {
        cout << "penning> ";
        return;
    }
    if (clear)
    {
        clear_cmd;
    }
    if (incorrect_input)
    {
        cout << "Incorret input, please, try again\n";
    }
    else if (current_status == "initial_menu")
    {
        cout << 
            "Write\n" <<
            "1) Start\n" <<
            "2) Exit\n";
    }
    else if (current_status == "main_menu")
    {
        cout << "Choose configuration:\n\n";

        for (int i = 0; i < configurations.size(); ++i)
        {
            cout << i + 1 << ") " << configurations[i] << endl;
        }
        cout << configurations.size() + 1 << ") New configuration ...\n" <<
                configurations.size() + 2 << ") Exit\n";
    }
    else if (current_status == "new_config")
    {
        cout << "Variables:" << endl;
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
                "electron\n" <<
                "positron\n" <<
                "proton\n" <<
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
                "ellipse <value> (lies in [-1,1])\n" <<
                "ampl <value> (must be > 0)\n" <<
                "freq <value> (must be > 0)\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->M_field")
    {
        cout << "Write:\n" << 
                "1) cartesian\n" <<
                "2) spherical\n" <<
                "3) exit\n";
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
                "(angles are in degrees)\n" <<
                "exit\n";
    }
    else if (current_status == "new_config->geometry")
    {
        cout << "Write:\n" << 
                "1) size\n" <<
                "2) coord\n" <<
                "3) vel\n" <<
                "4) exit\n";
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
    else if (current_status == "new_config->save->rewrite")
    {
        cout << "File with this name is already exist.\n" <<
                "Do you want to rewrite? [y/n]\n";
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
                "<time value>\n" <<
                "del\n" <<
                "exit\n";
    }
    else if (current_status == "pre-launch_window->delete")
    {
        cout << "Are you sure you want to delete this configuration? [y/n]\n";
    }
    else if (current_status == "model")
    {
        cout << model_name << " was successfully modeled\n";

        cout << "write:\n" <<
                "1) 3d (plot)\n" <<
                "2) <two coordinates for projection>\n" <<
                "3) <one coordinate for time dependency>\n" <<
                "4) exit\n";
    }
    cout << "penning> ";
    return;
}

void system_configuration::get_request()
{
    std::getline(cin, income_command);

    incorrect_input = false;
    clear = true;
    print_ = true;

    {
        int last = income_command.find_last_not_of(' ') == std::string::npos ? 0 : income_command.find_last_not_of(' ');
        income_command = income_command.substr(0, last + 1);
        int first = income_command.find_first_not_of(' ') == std::string::npos ? 0 : income_command.find_first_not_of(' ');
        income_command = income_command.substr(first, income_command.size() - first + 1);
    }
    if (income_command == "")
    {
        print_ = false;
        clear = false;
        return;
    }
    else if (income_command == ".q")
    {
        current_status = "exit";
        return;
    }
    else if (income_command == "clear" || income_command == "clean")
    {
        clear = true;
        return;
    }
    else if (current_status == "initial_menu")
    {
        if (income_command == "1" || income_command == "start")
        {
            current_status = "main_menu";
            return;
        }
        else if (income_command == "2" || income_command == "exit")
        {
            current_status = "exit";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (current_status == "main_menu")
    {
        int i = -1;
        if (income_command == std::to_string(configurations.size() + 2) || income_command == "Exit" || income_command == "exit")
        {
            current_status = "initial_menu";
            return;
        }
        else if (income_command == "new" || income_command == "New")
        {
            current_status = "new_config";
            return;
        }
        try{
            i = std::stoi(income_command);
            if (i >= 1 && i <= configurations.size())
            {
                model_name = configurations[i - 1];
                current_status = "pre-launch_window";
                read_configuration(configurations[i - 1]);
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
    else if (current_status == "new_config")
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
    else if (current_status == "new_config->particle")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else if (income_command == "electron")
        {
            model_config["MASS"] = 1;
            model_config["CHARGE"] = -1;
            current_status = "new_config";
            return;
        }
        else if (income_command == "positron")
        {
            model_config["MASS"] = 1;
            model_config["CHARGE"] = 1;
            current_status = "new_config";
            return;
        }
        else if (income_command == "proton")
        {
            model_config["MASS"] = 1836;
            model_config["CHARGE"] = 1;
            current_status = "new_config";
            return;
        }
        double par = 0;
        try {
            int i = income_command.find_last_of(' ');
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
    else if (current_status == "new_config->E_field")
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
    else if (current_status == "new_config->E_field->static")
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
    else if (current_status == "new_config->E_field->rotating")
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
    else if (current_status == "new_config->M_field")
    {
        if (income_command == "1" || income_command == "cartesian")
        {
            current_status = "new_config->M_field->cartesian";
            return;
        }
        else if (income_command == "2" || income_command == "spherical")
        {
            current_status = "new_config->M_field->spherical";
            return;
        }
        else if (income_command == "3" || income_command == "exit" || income_command == "Exit")
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
    else if (current_status == "new_config->M_field->cartesian")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        else
        {
            try {
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

                current_status = "new_config";
                return;
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
    }
    else if (current_status == "new_config->M_field->spherical")
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
                model_config["M_X"] = r * sin(theta * 180 / M_PI) * cos(phi * 180 / M_PI);
                model_config["M_Y"] = r * sin(theta * 180 / M_PI) * sin(phi * 180 / M_PI);
                model_config["M_Z"] = r * cos(theta * 180 / M_PI);

                current_status = "new_config";
                return;
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
    }
    else if (current_status == "new_config->geometry")
    {
        if (income_command == "1" || income_command == "size" || income_command == "Size")
        {
            current_status = "new_config->geometry->size";
            return;
        }
        else if (income_command == "2" || income_command == "coord" || income_command == "Coord" || income_command == "coordinate" || income_command == "Coordinate")
        {
            current_status = "new_config->geometry->coordinate";
            return;
        }
        else if (income_command == "3" || income_command == "velocity" || income_command == "Velovity" || income_command == "vel" || income_command == "Vel")
        {
            current_status = "new_config->geometry->velocity";
            return;
        }
        else if (income_command == "4" || income_command == "exit" || income_command == "Exit")
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
    else if (current_status == "new_config->geometry->size")
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
    else if (current_status == "new_config->geometry->coordinate")
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

                current_status = "new_config";
                return;
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
    }
    else if (current_status == "new_config->geometry->velocity")
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

                current_status = "new_config";
                return;
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
        }
    }
    else if (current_status == "new_config->save")
    {
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
        model_name = income_command;
        bool rewrite = false;
        for (int i = 0; i < configurations.size(); ++i)
            rewrite |= (income_command == configurations[i]);
        if (rewrite)
        {
            current_status = "new_config->save->rewrite";
            return;
        }
        std::ofstream out("src/penning/saved_configurations/"+income_command+".txt");
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
    else if (current_status == "new_config->save->rewrite")
    {
        if (income_command == "y" || income_command == "yes" || income_command == "Y" || income_command == "Yes")
        {
            std::ofstream out("src/penning/saved_configurations/"+model_name+".txt");
            if (!out.good())
            {
                out.close();
                clear = false;
                incorrect_input = true;
                return;
            }
            save_model(out);
            out.close();
            current_status = "main_menu";
            reset_config();
            return;
        }
        else if (income_command == "n" || income_command == "no" || income_command == "N" || income_command == "No")
        {
            current_status = "new_config->save";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (current_status == "new_config->exit")
    {
        if (income_command == "y" || income_command == "yes" || income_command == "Y" || income_command == "Yes")
        {
            current_status = "main_menu";
            reset_config();
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
    else if (current_status == "pre-launch_window")
    {
        if (income_command == "del" || income_command == "Del" || income_command == "delete" || income_command == "Delete")
        {
            current_status = "pre-launch_window->delete";
            return;
        }
        else if (income_command == "exit" || income_command == "Exit")
        {
            reset_config();
            current_status = "main_menu";
            return;
        }
        try {
            double time = std::stod(income_command);
            count(time);
            current_status = "model";
            return;
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
    else if (current_status == "pre-launch_window->delete")
    {
        if (income_command == "y" || income_command == "yes" || income_command == "Y" || income_command == "Yes")
        {
            int idx = -1;
            for (int i = 0; i < configurations.size(); ++i)
                if (model_name == configurations[i])
                    idx = i;
            configurations.erase(configurations.begin() + idx);
            try {
                std::string cmd_s = "rm src/penning/saved_configurations/"+model_name+".txt";
                char cmd[cmd_s.size()];
                for (int i = 0; i < cmd_s.size(); ++i)
                    cmd[i] = cmd_s[i];
                system(cmd);
                current_status = "main_menu";
                return;
            }
            catch (...) {
                cout << "Cannot run rm src/penning/saved_configurations/"+model_name+".txt\nFATAL ERROR\n";
                current_status = "pre-launch_window";
                return;
            }
        }
        else if (income_command == "n" || income_command == "no" || income_command == "N" || income_command == "No")
        {
            current_status = "pre-launch_window";
            return;
        }
        else
        {
            clear = false;
            incorrect_input = true;
            return;
        }
    }
    else if (current_status == "model")
    {
        if (income_command == "3d" or income_command == "3d plot")
        {
            matplot::xlabel("x");
            matplot::ylabel("y");
            matplot::zlabel("z");
            matplot::plot3(x, y, z);
            matplot::show();
            return;
        }
        else if (income_command == "xy" || income_command == "XY")
        {
            matplot::xlabel("x");
            matplot::ylabel("y");
            matplot::grid(true);
            matplot::plot(x, y);
            matplot::show();
            return;
        }
        else if (income_command == "x" || income_command == "X")
        {
            matplot::xlabel("t");
            matplot::ylabel("x");
            matplot::grid(true);
            matplot::plot(t, x);
            matplot::show();
            return;
        }
        else if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "pre-launch_window";
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
    clear_cmd;

    std::ofstream out("src/penning/saved_configurations.txt");
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

    return;
}