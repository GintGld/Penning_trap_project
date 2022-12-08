#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include "vector3d.h"
#include "constants.h"

////////////////////////////////////////
//            Declaration             //
////////////////////////////////////////

template<typename T>
struct particle final
{
private:
    vector3d<T> r;
    vector3d<T> v;
    const T charge, mass;
    const std::string special_type;
    
    particle(vector3d<T> r, vector3d<T> v, T charge, T mass, std::string special_type);

public:
    vector3d<T> get_position() const;
    vector3d<T> get_velocity() const;
    T get_charge() const;
    T get_mass() const;

    particle operator=(const particle p_new);

    void set_position(const vector3d<T>& r_new);
    void set_velocity(const vector3d<T>& v_new);

    template<typename T_for_el>
    friend particle<T_for_el> new_electron(vector3d<T_for_el> r, vector3d<T_for_el> v);

    template<typename T_for_prot>
    friend particle<T_for_prot> new_proton(vector3d<T_for_prot> r, vector3d<T_for_prot> v);

    template<typename T_for_custom>
    friend particle<T_for_custom> new_particle(
        vector3d<T_for_custom> r, 
        vector3d<T_for_custom> v,
        T_for_custom charge,
        T_for_custom mass);

    template<typename T_for_str>
    friend std::string to_str(const particle<T_for_str> p);

    template<typename T_for_ostream>
    friend std::ostream & operator << (std::ostream& os, const particle<T_for_ostream>& p);
};

////////////////////////////////////////
//             Definition             //
////////////////////////////////////////

template<typename T>
particle<T>::particle(
        vector3d<T> r_, 
        vector3d<T> v_, 
        T charge_, 
        T mass_,
        std::string special_type):
        r(r_), v(v_), charge(charge_), mass(mass_), special_type(special_type) {}

template<typename T>
vector3d<T>
particle<T>::get_position() const
{
    return r;
}

template<typename T>
vector3d<T>
particle<T>::get_velocity() const
{
    return v;
}

template<typename T>
T
particle<T>::get_charge() const
{
    return charge;
}

template<typename T>
T
particle<T>::get_mass() const
{
    return mass;
}

template<typename T>
void
particle<T>::set_position(const vector3d<T>& r_new)
{
    r = r_new;
    return;
}

template<typename T>
void
particle<T>::set_velocity(const vector3d<T>& v_new)
{
    v = v_new;
    return;
}

template<typename T>
particle<T>
particle<T>::operator=(const particle<T> p_new)
{
    if (charge != p_new.charge || mass != p_new.mass || special_type != p_new.special_type)
    {
        throw("Particles' parameters are not the same.\nThis operator can change only r-vector and v-vector");
    }
    r = p_new.r;
    v = p_new.v;
    return *this;
}

template<typename T>
particle<T> new_electron(vector3d<T> r, vector3d<T> v)
{
    particle<T> tmp(r, v, (T)(ELECTRON_CHARGE), (T)(ELECTRON_MASS), "electron");
    return tmp;
}

template<typename T>
particle<T> new_electron(vector3d<T> r)
{
    return new_electron<T>(r, vector3d<T>(0,0,0));
}

template<typename T>
particle<T> new_electron()
{
    return new_electron<T>(vector3d<T>(0,0,0));
}

template<typename T>
particle<T> new_proton(vector3d<T> r, vector3d<T> v)
{
    particle<T> tmp(r, v, (T)(PROTON_CHARGE), (T)(PROTON_MASS), "proton");
    return tmp;
}

template<typename T>
particle<T> new_proton(vector3d<T> r)
{
    return new_proton<T>(r, vector3d<T>(0,0,0));
}

template<typename T>
particle<T> new_proton()
{
    return new_proton<T>(vector3d<T>(0,0,0));
}

template<typename T>
particle<T> new_particle(vector3d<T> r, vector3d<T> v, T charge, T mass)
{
    particle<T> tmp(r, v, charge, mass, "undefined");
    return tmp;
}

template<typename T>
particle<T> new_particle(vector3d<T> r, T charge, T mass)
{
    return new_particle<T>(r, vector3d<T>(0,0,0), charge, mass);
}

template<typename T>
particle<T> new_particle(T charge, T mass)
{
    return new_particle<T>(vector3d<T>(0,0,0), charge, mass);
}

template<typename T>
std::string to_str(const particle<T> p)
{
    return "particle: " + p.special_type + "\nr: " + to_str(p.get_position()) + "\nv: " + to_str(p.get_velocity()) + 
            "\ncharge: " + std::to_string(p.get_charge()) + 
            "\nmass : " + std::to_string(p.get_mass()) + "\n";
}

template<typename T>
std::ostream & operator << (std::ostream& os, const particle<T>& p)
{
    return os << to_str<T>(p);
}

#endif // PARTICLE_H