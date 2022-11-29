#ifndef FIELD_H
#define FIELD_H

#include "vector3d.h"

template<typename T>
struct field
{
private:
    // Function that defines vector field
    vector3d<T> (*f)(vector3d<T>);

public:
    // default constructor, defines function as 0
    field();
    field(vector<T> (*f)(vector<T>));

    // changes function
    void set(vector<T> (*f)(vector<T>));

    vector3d<T> operator()(vector3d<T> r);
};

/*
    Definitions of Penning Trap fields (2 or 3) 
    OR
    Functions that construct these fields
*/

#endif // FIELD_H