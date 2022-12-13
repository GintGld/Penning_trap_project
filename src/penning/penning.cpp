#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>

#include "penning.h"
#include "phys/phys.h"
#include "matplot/matplot.h"

using std::cin;
using std::cout;
using std::endl;
#define clear_cmd system("clear")

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

template <typename T>
field<T> penning_E(T wz, T eps, T A, T wr, particle<T> p)
{
    field<T> tmp = new_field<T>();
    if (wz != 0)
    {
        auto stat =  [wz, eps, p](T t, vector3d<T> r){
            return (.5 * wz * p.get_mass() / p.get_charge()) * vector3d<T>(
                (1 + eps) * r.x(),
                (1 - eps) * r.y(),
                -2 * r.z());
        };
        tmp += new_field<T>(stat);
    }
    if (A != 0 && wr != 0)
    {
        auto rot = [A, wr, p](T t, vector3d<T> r){
            return (A * r.z() * sqrt(r.x()*r.x() + r.y()*r.y()) * p.get_mass() / p.get_charge()) *
            vector3d<T> (
                -r.z() * cos(wr * t),
                r.z() * sin(wr * t),
                r.x() * cos(wr * t) - r.y() * sin(wr * t)
            );
        };
        tmp += new_field<T>(rot);
    }
    return tmp;
}

template <typename T>
field<T> penning_M(vector3d<T> B)
{
    return new_field(B);
}

system_configuration::system_configuration()
{
    read_saved_configurations();
    reset_config();
    create_binary_dir();
    current_status = "initial_menu";
}

void system_configuration::read_saved_configurations()
{
    std::string name;
    try {
        system("ls src/penning/saved_configurations/ > src/penning/ls_list.txt");
    }
    catch (...) {
        cout << "Cannot run ls src/penning/saved_configurations/ > src/penning/ls_list.txt\n";
        return;
    }
    std::ifstream in("src/penning/ls_list.txt");
    if (!in.good())
    {
        in.close();
        cout << "Cannot open src/penning/ls_list.txt\n";
        return;
    }

    while(in)
    {
        std::getline(in, name);
        if (name == "")
            continue;
        if (name.substr(name.size() - 4, 4) == ".txt")
        {
            configurations.push_back(name.substr(0, name.size() - 4));
        }
    }

    in.close();
    try {
        system("rm src/penning/ls_list.txt");
    }
    catch (...) {
        cout << "Cannot run rm src/penning/ls_list.txt\n";
        return;
    }
    return;
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
    model_config["FREQ_Z"] = 1;
    model_config["ELL"] = 0;
    model_config["AMPL"] = 0;
    model_config["FREQ_R"] = 0;
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
    time_step = .01;
    n_repeat = 10;
    return;
}

void system_configuration::create_binary_dir()
{
    try {
        system("mkdir src/penning/binary");
        return;
    }
    catch (...) {
        cout << "Cannot run 'mkdir src/penning/binary'\n";
        return;
    }
}

void system_configuration::delete_binary_dir()
{
    try {
        system("rm -rf src/penning/binary");
        return;
    }
    catch (...) {
        cout << "Cannot run 'rm -rf src/penning/binary'\n";
        return;
    }
}

bool system_configuration::is_config_changed()
{
    bool f = false;
    f |= (model_config["CHARGE"] != 1);
    f |= (model_config["MASS"] != 1);
    f |= (model_config["FREQ_Z"] != 1);
    f |= (model_config["ELL"] != 0);
    f |= (model_config["AMPL"] != 0);
    f |= (model_config["FREQ_R"] != 0);
    f |= (model_config["M_X"] != 0);
    f |= (model_config["M_Y"] != 0);
    f |= (model_config["M_Z"] != 10);
    f |= (model_config["SIZE"] != 20);
    f |= (model_config["X"] != 1);
    f |= (model_config["Y"] != 0);
    f |= (model_config["Z"] != 0);
    f |= (model_config["V_X"] != 0);
    f |= (model_config["V_Y"] != 1);
    f |= (model_config["V_Z"] != 0);
    return f;
}

void system_configuration::print_model()
{
    cout.precision(3);
    cout << "----------------------------------" << endl <<
            "Particle\n" <<
            "Mass:\t\t\t\t" << model_config["MASS"] << endl <<
            "Charge:\t\t\t\t" << model_config["CHARGE"] << endl <<
            "----------------------------------" << endl << 
            "Electrical field\n" <<
            "Axial fluctiation frequency:\t" << model_config["FREQ_Z"] << endl <<
            "Ellipticity parameter:\t\t" << model_config["ELL"] << endl <<
            "Static:\t\t\t\t" << ((model_config["AMPL"] == 0 && model_config["FREQ_R"] == 0) ? "YES" : "NO") << endl <<
            "Rotation amplitude:\t\t" << model_config["AMPL"] << endl <<
            "Rotation frequency:\t\t" << model_config["FREQ_R"] << endl <<
            "----------------------------------" << endl << 
            "Magnetic field\t\t\t" <<
            "(" << model_config["M_X"] << ", " << model_config["M_Y"] << ", " << model_config["M_Z"] << ")\n" <<
            "----------------------------------" << endl << 
            "Geometry\n" <<
            "Detector size:\t\t\t" << model_config["SIZE"] << endl <<
            "Particle posotion:\t\t" << 
            "(" << model_config["X"] << ", " << model_config["Y"] << ", " << model_config["Z"] << ")\n" <<
            "Particle velocity:\t\t" <<
            "(" << model_config["V_X"] << ", " << model_config["V_Y"] << ", " << model_config["V_Z"] << ")\n" <<
            "----------------------------------" << endl;
    return;
}

void system_configuration::save_model(std::ofstream& out)
{
    out <<
        "CHARGE : "   << model_config["CHARGE"] << endl << 
        "MASS : "     << model_config["MASS"] << endl << 
        "FREQ_Z : "      << model_config["FREQ_Z"] << endl << 
        "ELL : "    << model_config["ELL"] << endl << 
        "AMPL : " << model_config["AMPL"] << endl << 
        "FREQ_R : " << model_config["FREQ_R"] << endl << 
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
    return;
}

void system_configuration::count(double time)
{
    model_space.set_particle( new_particle<double>(
        vector3d<double> (model_config["X"], model_config["Y"], model_config["Z"]),
        vector3d<double> (model_config["V_X"], model_config["V_Y"], model_config["V_Z"]),
        model_config["CHARGE"], model_config["MASS"]));
    model_space.set_E_field( penning_E<double>(
        model_config["FREQ_Z"],
        model_config["ELL"],
        model_config["AMPL"],
        model_config["FREQ_R"],
        model_space.get_particle()));
    model_space.set_M_field( penning_M<double>(
        vector3d<double>(model_config["M_X"], model_config["M_Y"], model_config["M_Z"])));

    //if (model_config["AMPL"] == 0 && model_config["FREQ_R"] == 0)
        n_repeat = (unsigned)(1000 * time_step);
    //else
    //    n_repeat = (unsigned)(50 * time * time_step);

    model_space.solve(time, time_step, "rk45", n_repeat);
    model_space.write_T("src/penning/binary/"+model_name);
    model_space.clear();

    x.clear(); y.clear(); z.clear();
    vx.clear(); vy.clear(); vz.clear();
    t.clear();

    double* r = new double[6];
    is_out_of_borders = false;
    std::ifstream in("src/penning/binary/"+model_name+".binary", std::ios::binary);
    if (!in.good())
    {
        cout << "Cannot open src/penning/binary/"<<model_name<<"\nFATAL ERROR\n";
        return;
    }
    for (int i = 0; i <= (unsigned)(time / .01) && !is_out_of_borders; ++i)
    {
        for (int j = 0; j < 6; ++j)
            in.read((char*) &r[j], sizeof(double));
        x.push_back(r[0]);
        y.push_back(r[1]);
        z.push_back(r[2]);
        vx.push_back(r[3]);
        vy.push_back(r[4]);
        vz.push_back(r[5]);
        t.push_back((double)(.01 * i));

        if (2 * x[i] - model_config["SIZE"] >= 0 ||
            2 * x[i] + model_config["SIZE"] <= 0 ||
            2 * y[i] - model_config["SIZE"] >= 0 ||
            2 * y[i] + model_config["SIZE"] <= 0 ||
            2 * z[i] - model_config["SIZE"] >= 0 ||
            2 * z[i] + model_config["SIZE"] <= 0)
        {
            is_out_of_borders = true;
            break;
        }
    }
    delete[] r;
    in.close();
    return;
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
        cout << "1) Start\n" <<
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
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "-- electron\n" <<
                "-- positron\n" <<
                "-- proton\n" <<
                "-- mass <value>\n" <<
                "-- charge <value>\n" <<
                "-- exit\n";
    }
    else if (current_status == "new_config->E_field")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "-- wz <value> (must be > 0)\n" <<
                "-- ell <value> (lies in [-1,1])\n" <<
                "-- amp <value> (must be > 0)\n" <<
                "-- wr <value> (must be > 0)\n" <<
                "-- exit\n";
    }
    else if (current_status == "new_config->M_field")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "1) cartesian\n" <<
                "2) spherical\n" <<
                "3) exit\n";
    }
    else if (current_status == "new_config->M_field->cartesian")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "-- <M_x> <M_y> <M_z>\n" <<
                "-- exit\n";
    }
    else if (current_status == "new_config->M_field->spherical")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "-- <M_r> <M_theta> <M_phi>\n" <<
                "-- (angles are in degrees)\n" <<
                "-- exit\n";
    }
    else if (current_status == "new_config->geometry")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "1) size\n" <<
                "2) coordinate (coord)\n" <<
                "3) velocity (vel)\n" <<
                "4) exit\n";
    }
    else if (current_status == "new_config->geometry->size")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "-- <value>\n" <<
                "-- exit\n";
    }
    else if (current_status == "new_config->geometry->coordinate")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "-- <X> <Y> <Z>\n" <<
                "-- exit\n";
    }
    else if (current_status == "new_config->geometry->velocity")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Choose:\n" <<
                "-- <V_x> <V_y> <V_z>\n" <<
                "-- exit\n";
    }
    else if (current_status == "new_config->save")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Write the name\n";
    }
    else if (current_status == "new_config->save->rewrite")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "File with this name is already exist.\n" <<
                "Do you want to rewrite? [y/n]\n";
    }
    else if (current_status == "new_config->exit")
    {
        cout << "Variables:" << endl;
        print_model();
        cout << "Are you sure you want to exit and reset configuration? [y/n]\n";
    }
    else if (current_status == "pre-launch_window")
    {
        cout << "Configuration: " << model_name << endl;
        print_model();
        cout << "time step:\t\t\t" << time_step << endl <<
                "----------------------------------" << endl <<
                "Choose:\n" <<
                "-- <time value>\n" <<
                "-- step <time step>\n" <<
                "-- del\n" <<
                "-- exit\n";
    }
    else if (current_status == "pre-launch_window->delete")
    {
        cout << "Are you sure you want to delete this configuration? [y/n]\n";
    }
    else if (current_status == "model")
    {
        cout << model_name << " was successfully modeled\n";
        if (is_out_of_borders)
        {
            cout.precision(3);
            cout << "\nNote: at t = " << .01 * x.size() << " particle flew out of trap\n\n";
        }

        cout << "-- 3d (plot)\n" <<
                "-- <two coordinates for projection>\n" <<
                "-- <one coordinate for time dependency>\n" <<
                "-- back\n" <<
                "-- menu\n";
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
        
        if (income_command == "Exit" || income_command == "exit")
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
            int i = std::stoi(income_command);
            if (i >= 1 && i <= configurations.size())
            {
                model_name = configurations[i - 1];
                current_status = "pre-launch_window";
                read_configuration(configurations[i - 1]);
                return;
            }
            else if (i == configurations.size() + 1)
            {
                reset_config();
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
            if (is_config_changed())
                current_status = "new_config->exit";
            else
                current_status = "main_menu";
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
        if (income_command.substr(0,3) == "wz " && par >= 0)
        {
            model_config["FREQ_Z"] = par;
            current_status = "new_config";
            return;
        }
        else if (income_command.substr(0,4) == "ell " && par >= -1 && par <= 1)
        {
            model_config["ELL"] = par;
            current_status = "new_config";
            return;
        }
        else if (income_command.substr(0,4) == "amp " && par >= 0)
        {
            model_config["AMPL"] = par;
            current_status = "new_config";
            return;
        }
        else if (income_command.substr(0,3) == "wr " && par >= 0)
        {
            model_config["FREQ_R"] = par;
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
                model_config["M_X"] = r * sin(theta * M_PI / 180) * cos(phi * M_PI / 180);
                model_config["M_Y"] = r * sin(theta * M_PI / 180) * sin(phi * M_PI / 180);
                model_config["M_Z"] = r * cos(theta * M_PI / 180);

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
        if (income_command == "exit" || income_command == "Exit")
        {
            current_status = "new_config";
            return;
        }
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
        else if (income_command.substr(0, 5) == "step ")
        {
            try {
                int i = income_command.find(' ');
                cout << income_command.substr(i+1, income_command.size() - i) << endl;
                time_step = std::stod(income_command.substr(i+1, income_command.size() - i));
                return;
            }
            catch (...) {
                clear = false;
                incorrect_input = true;
                return;
            }
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
            current_status = "main_menu";
            return;
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
        else if (income_command == "xz" || income_command == "XZ")
        {
            matplot::xlabel("x");
            matplot::ylabel("z");
            matplot::grid(true);
            matplot::plot(x, z);
            matplot::show();
            return;
        }
        else if (income_command == "yx" || income_command == "YX")
        {
            matplot::xlabel("y");
            matplot::ylabel("x");
            matplot::grid(true);
            matplot::plot(y, x);
            matplot::show();
            return;
        }
        else if (income_command == "yz" || income_command == "YZ")
        {
            matplot::xlabel("y");
            matplot::ylabel("z");
            matplot::grid(true);
            matplot::plot(y, z);
            matplot::show();
            return;
        }
        else if (income_command == "zx" || income_command == "ZX")
        {
            matplot::xlabel("z");
            matplot::ylabel("x");
            matplot::grid(true);
            matplot::plot(z, x);
            matplot::show();
            return;
        }
        else if (income_command == "zy" || income_command == "ZY")
        {
            matplot::xlabel("z");
            matplot::ylabel("y");
            matplot::grid(true);
            matplot::plot(z, y);
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
        else if (income_command == "y" || income_command == "Y")
        {
            matplot::xlabel("t");
            matplot::ylabel("y");
            matplot::grid(true);
            matplot::plot(t, y);
            matplot::show();
            return;
        }
        else if (income_command == "z" || income_command == "Z")
        {
            matplot::xlabel("t");
            matplot::ylabel("z");
            matplot::grid(true);
            matplot::plot(t, z);
            matplot::show();
            return;
        }
        else if (income_command == "back" || income_command == "Back")
        {
            x.clear(); y.clear(); z.clear();
            vx.clear(); vy.clear(); vz.clear();
            t.clear();
            current_status = "pre-launch_window";
            return;
        }
        else if (income_command == "menu" || income_command == "Menu")
        {
            reset_config();
            model_name = "undefined";
            current_status = "main_menu";
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
    std::string name;
    bool found;

    try {
        system("ls src/penning/saved_configurations/ > src/penning/ls_list.txt");
    }
    catch (...) {
        cout << "Cannot run ls src/penning/saved_configurations/ > src/penning/ls_list.txt\n";
        return;
    }
    std::ifstream in("src/penning/ls_list.txt");
    if (!in.good())
    {
        in.close();
        cout << "Cannot open src/penning/ls_list.txt\n";
        return;
    }

    while (in)
    {
        std::getline(in, name);
        if (name == "")
            continue;
        found = false;
        for (int i = 0; i < configurations.size(); ++i)
            found |= (name == (configurations[i] + ".txt"));
        if (!found)
        {
            try {
                std::string cmd_s = "rm src/penning/saved_configurations/"+name;
                char* cmd = new char[cmd_s.size()];
                for (int i = 0; i < cmd_s.size(); ++i)
                    cmd[i] = cmd_s[i];
                system(cmd);
                delete[] cmd;
            }
            catch (...) {
                cout << "Cannot run 'rm src/penning/saved_configurations/" << name << "'\n";
            }
        }
    }

    in.close();
    try {
        system("rm src/penning/ls_list.txt");
    }
    catch (...) {
        cout << "Cannot run rm src/penning/ls_list.txt\n";
        return;
    }
    delete_binary_dir();
    return;
}