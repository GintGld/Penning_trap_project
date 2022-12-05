#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector3d.h"

#define ELECTRON_CHARGE 1
#define ELECTRON_MASS 1

template<typename T>
struct particle final
{
private:
    vector3d<T> r;
    vector3d<T> v;
    T const charge, mass;

    //particle();
    //particle(vector3d<T> r, T charge, T mass);
    particle(
        vector3d<T> r = vector3d<T>(0,0,0), 
        vector3d<T> v = vector3d<T>(0,0,0), 
        T charge = ELECTRON_CHARGE, 
        T mass = ELECTRON_MASS);

public:
    void set_position(vector3d<T>& r_new);
    void set_velocity(vector3d<T>& v_new);

    friend particle<T> 
    new_electron(
        vector3d<T> r = vector3d<T>(0,0,0), 
        vector3d<T> v = vector3d<T>(0,0,0));
    friend particle<T> 
    new_proton(
        vector3d<T> r = vector3d<T>(0,0,0), 
        vector3d<T> v = vector3d<T>(0,0,0));
    friend particle<T> 
    new_particle(
        vector3d<T> r = vector3d<T>(0,0,0), 
        vector3d<T> v = vector3d<T>(0,0,0),
        T charge = (T)(ELECTRON_CHARGE), 
        T mass = (T)(ELECTRON_MASS));
};

/*
    Definitions of some example particles, e.g.:
    electron, proton.
    Function than constructs particle with given charge and mass
*/
template<typename T>
particle<T>
new_electron(
    vector3d<T> r = vector3d<T>(0,0,0), 
    vector3d<T> v = vector3d<T>(0,0,0));

template<typename T>
particle<T> 
new_proton(
    vector3d<T> r = vector3d<T>(0,0,0),
    vector3d<T> v = vector3d<T>(0,0,0));

template<typename T>
particle<T> 
new_particle(
    vector3d<T> r = vector3d<T>(0,0,0), 
    vector3d<T> v = vector3d<T>(0,0,0), 
    T charge = (T)(ELECTRON_CHARGE), 
    T mass = (T)(ELECTRON_MASS));

#endif // PARTICLE_H