#ifndef SPACE_H
#define SPACE_H

#include "vector3d.h"
#include "particle.h"
#include "field.h"

#include <vector>

template<typename T>
class space final
{
private:
    vector3d<T> space_size;
    field<T> E, M;
    particle<T> P;
    std::vector< std::pair<vector3d<T>, vector3d<T> > > P_history;

public:
    space(
        particle<T> P = new_particle(),
        field<T> E = new_field(),
        field<T> M = new_field(),
        vector3d<T> space_size = vector3d<T>(0,0,0)
    );
    particle<T>& get_particle();
    particle<T>  get_particle();
    field<T>&    get_E_field();
    field<T>     get_E_field();
    field<T>&    get_M_field();
    field<T>     get_M_field();
    vector3d<T>& get_size();
    vector3d<T>  get_size();
    void solve(T time, T dt);
    void write();
};

template<typename T>
particle<T> next_step(particle<T> p, field<T> E, field<T> M);
template<typename T>
particle<T> n_step(particle<T> p, field<T> E, field<T> M, unsigned n);

#endif // SPACE_H