#ifndef SPACE_H
#define SPACE_H

#include <string>
#include <fstream>

#include "vector3d.h"
#include "narray.h"
#include "solving_methods.h"
#include "constants.h"
#include "particle.h"
#include "field.h"

////////////////////////////////////////
//            Declaration             //
////////////////////////////////////////

template<typename T>
class space final
{
private:
    // Inner Variables
    vector3d<T> space_size;
    field<T> E, M;
    particle<T> P;
    narray<narray<vector3d<T> > > history;
    
    // Private Constructors
    space(
        particle<T> P,
        field<T> E,
        field<T> M,
        vector3d<T> space_size
    );
    space();

public:
    // Methods
    particle<T> get_particle() const;
    field<T>    get_E_field() const;
    field<T>    get_M_field() const;
    vector3d<T> get_size() const;
    size_t get_history_size() const;
    void set_particle(particle<T> p);
    void set_E_field(field<T> f);
    void set_M_field(field<T> f);
    void set_size(vector3d<T> v);
    void add_to_history(narray<vector3d<T> > v);
    void solve(T time, T dt, std::string type, unsigned N_repeat);
    int write(std::string file);
    void clear();

    // Function for constructing custom space
    template<typename T_new>
    friend space<T_new> new_space(particle<T_new> p, field<T_new> E, field<T_new> M, vector3d<T_new> space_size);

    // Function that make step of modeling for one particle
    template<typename T1>
    friend void
    make_step(
        particle<T>& p,
        space<T>& S,
        T t,
        T h,
        std::string type, unsigned N
    );
};

////////////////////////////////////////
//            Definition              //
////////////////////////////////////////

////////////////////////////////////////
//           Constructors             //
////////////////////////////////////////

template<typename T>
space<T>::space(particle<T> P, field<T> E, field<T> M, vector3d<T> space_size):
    P(P), E(E), M(M), space_size(space_size) {}

template<typename T>
space<T>::space() {}

////////////////////////////////////////
//             Methods                //
////////////////////////////////////////

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
size_t space<T>::get_history_size() const
{
    return history.size();
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
void space<T>::add_to_history(narray<vector3d<T> > v)
{
    this->history.push_back(v);
    return;
}

template<typename T>
void space<T>::clear()
{
    history.clear();
    return;
}

////////////////////////////////////////
//      Functions that construct      //
//            new spaces              //
////////////////////////////////////////

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

////////////////////////////////////////
//       Functions for modeling       //
////////////////////////////////////////

template<typename T>
void make_step(particle<T>& p, space<T>& S, T t, T h, std::string type, unsigned N)
{
    vector3d<T> r = p.get_position(), v = p.get_velocity();
    field<T> E = S.get_E_field(), M = S.get_M_field();
    std::function<narray<vector3d<T> >(T, narray<vector3d<T> >)>
    f = [p, E, M](T time, narray<vector3d<T> > arr){
        return narray<vector3d<T> >
            {arr[1], (p.get_charge() / p.get_mass()) * (E(arr[0]) + ((arr[1] / SPEED_OF_LIGHT) ^ M(arr[0])))};
    };
    narray<vector3d<T> > arr = step(t, narray<vector3d<T> > {r, v}, f, h, type, N);
    p.set_position(arr[0]);
    p.set_velocity(arr[1]);
    S.add_to_history(arr);
    return;
}

template<typename T>
void space<T>::solve(T time, T dt, std::string type, unsigned N_repeat)
{
    this->add_to_history(narray<vector3d<T> > {P.get_position(), P.get_velocity()});
    unsigned N = (unsigned)(time / dt);
    for (unsigned i = 0; i < N; ++i)
    {
        make_step(P, *this, static_cast<T>(i)*dt, dt, type, N_repeat);
    }
    /*
        ToDo:
        Add constant that constraints the history size
        and call write method whet it's needed
    */
    return;
}

template<typename T>
int space<T>::write(std::string file)
{
    std::ofstream out(file, std::ios::binary);

    if(!out.good())
        return EXIT_FAILURE;

    for (unsigned i = 0; i < history.size(); ++i)
    {
        out.write((char*) &history[i][0], sizeof(vector3d<T>));
        out.write((char*) &history[i][1], sizeof(vector3d<T>));
    }
    out.close();
    return EXIT_SUCCESS;
}

#endif // SPACE_H