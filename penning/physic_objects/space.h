#ifndef SPACE_H
#define SPACE_H

#include "vector3d.h"
#include "constants.h"
#include "particle.h"
#include "field.h"

#include <vector>

////////////////////////////////////////
//            Declaration             //
////////////////////////////////////////

template<typename T>
class space final
{
private:
    vector3d<T> space_size;
    field<T> E, M;
    particle<T> P;
    std::vector< std::pair<vector3d<T>, vector3d<T> > > P_history;
    
    space(
        particle<T> P,
        field<T> E,
        field<T> M,
        vector3d<T> space_size
    );
    space();

public:
    particle<T> get_particle() const;
    field<T>    get_E_field() const;
    field<T>    get_M_field() const;
    vector3d<T> get_size() const;
    void set_particle(particle<T> p);
    void set_E_field(field<T> f);
    void set_M_field(field<T> f);
    void set_size(vector3d<T> v);
    void solve(T time, T dt);
    void write();

    template<typename T_new>
    friend space<T_new> new_space(particle<T_new> p, field<T_new> E, field<T_new> M, vector3d<T_new> space_size);
};

template<typename T>
particle<T> next_step(particle<T> p, field<T> E, field<T> M);
template<typename T>
particle<T> n_step(particle<T> p, field<T> E, field<T> M, unsigned n);

////////////////////////////////////////
//            Definition              //
////////////////////////////////////////

template<typename T>
space<T>::space(
        particle<T> P,
        field<T> E,
        field<T> M,
        vector3d<T> space_size
    ):
    P(P), E(E), M(M), space_size(space_size) {}

template<typename T>
space<T>::space() {}

template<typename T>
particle<T> space<T>::get_particle() const
{
    return P;
}

template<typename T>
field<T> space<T>::get_E_field() const
{
    return E;
}

template<typename T>
field<T> space<T>::get_M_field() const
{
    return M;
}

template<typename T>
vector3d<T> space<T>::get_size() const
{
    return space_size;
}

template<typename T>
void space<T>::set_particle(particle<T> P_)
{
    P = P_;
    return;
}

template<typename T>
void space<T>::set_E_field(field<T> f)
{
    E = f;
    return;
}

template<typename T>
void space<T>::set_M_field(field<T> f)
{
    M = f;
    return;
}

template<typename T>
void space<T>::set_size(vector3d<T> v)
{
    space_size = v;
    return;
}

template<typename T>
space<T> new_space(particle<T> p, field<T> E, field<T> M, vector3d<T> space_size)
{
    space<T> tmp(p, E, M, space_size);
    return tmp;
}

template<typename T>
space<T> new_space(field<T> E, field<T> M, vector3d<T> space_size)
{
    space<T> tmp;
    tmp.set_E_field(E);
    tmp.set_M_field(M);
    tmp.set_size(space_size);
    return tmp;
}

template<typename T>
space<T> new_space()
{
    space<T> tmp;
    return tmp;
}

#endif // SPACE_H