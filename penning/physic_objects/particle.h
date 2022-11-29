#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector3d.h"

template<typename T>
struct particle
{
private:
    vector3d<T> r;
    vector3d<T> v;
    T const charge, mass;

public:
    particle();
    particle(vector3d<T> r, T charge, T mass);
    particle(vector3d<T> r, vector3d<T> v, T charge, T mass);

    void set_position(vector3d<T>& r_new);
    void set_velocity(vector3d<T>& v_new);


};

/*
    Definitions of some example particles, e.g.:
    electron, proton.
    Function than constructs particle with given charge and mass
*/

#endif // PARTICLE_H