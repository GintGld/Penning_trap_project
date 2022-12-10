#include <iostream>
#include <map>

#include "penning/penning.h"
#include "phys/phys.h"
#include "matplot/matplot.h"


using std::cin;
using std::cout;
using std::endl; 


int main()
{
    //system_configuration system_status; // main variable

    cout << sum(3, 4) << endl;

    //system("clear");
    return 0;

    /*auto E = penning_E<double>(1, 0);
    auto M = penning_M<double>(vector_d (0,0,10));
    auto p = new_particle<double>(vector_d(0,0,0), vector_d(1,0,1), 1, 1);

    auto s = new_space(p, E, M, vector_d (1,1,1));

    double 
        time = 100,
        dt = .01,
        N_repeat = 10;

    s.solve(time, dt, "rk45", N_repeat);
    s.write("test.binary");

    std::vector<double> x, y, z, vx, vy, vz;
    vector_d r;

    std::ifstream in("test.binary", std::ios::binary);

    if (!in.good())
        return EXIT_FAILURE;

    for (int i = 0; i < (unsigned)(time / dt); ++i)
    {
        in.read((char*) &r, sizeof(vector_d));
        x.push_back(r.x());
        y.push_back(r.y());
        z.push_back(r.z());
        in.read((char*) &r, sizeof(vector_d));
        vx.push_back(r.x());
        vy.push_back(r.y());
        vz.push_back(r.z());
    }

    in.close();


    matplot::xlabel("x");
    matplot::ylabel("y");
    matplot::zlabel("z");

    matplot::plot3(x, y, z);
    matplot::show();

    return EXIT_SUCCESS;*/
}