#ifndef FIELD_H
#define FIELD_H

#include "vector3d.h"

template<typename T>
struct field final
{
private:
    // Function that defines vector field
    vector3d<T> (*f)(vector3d<T>);

    // constructor
    field(vector<T> (*f)(vector<T>) = [](vector3d<T> r)(return vector3d<T>(0,0,0);));

public:
    // changes function
    void set_function(vector<T> (*f)(vector<T>));
    
    vector3d<T> operator()(vector3d<T> r);

    friend template<typename T>
    field<T> new_penning_1d();

    friend template<typename T>
    field<T> new_penning_2d();

    friend template<typename T>
    field<T> new_penning_3d();

    friend template<typename T>
    field<T> new_field(vector3d<T> (*f)(vector3d<T>) = [](vector3d<T> r)(return vector3d<T>(0,0,0);));
};

/*
    Definitions of Penning Trap fields (2 or 3) 
    OR
    Functions that construct these fields
*/

template<typename T>
field<T> new_penning_1d();

template<typename T>
field<T> new_penning_2d();

template<typename T>
field<T> new_penning_3d();

template<typename T>
field<T> new_field(vector3d<T> (*f)(vector3d<T>) = [](vector3d<T> r)(return vector3d<T>(0,0,0);));

#endif // FIELD_H