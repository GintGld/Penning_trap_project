#ifndef SPACE_H
#define SPACE_H

#include "vector3d.h"
#include "particle.h"
#include "field.h"

#include <vector>

template<typename T>
class space
{
private:
    vector3d<T> space_size;
    field<T> E, M;
    particle<T> P;
    std::vector< std::pair<vector3d<T>, vector3d<T> > > P_history;

public:
    space();
    space(
        particle<T> P,
        field<T> E,
        field<T> M,
        vector3d<T> space_size
    );
    void solve(T time, T dt);
    void write();
};

template<typename T>
particle<T> next_step(particle<T> p, field<T> E, field<T> M);
template<typename T>
particle<T> n_step(particle<T> p, field<T> E, field<T> M, unsigned n);

#endif // SPACE_H