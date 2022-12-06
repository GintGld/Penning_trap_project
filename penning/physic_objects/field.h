#ifndef FIELD_H
#define FIELD_H

#include <cmath>
#include <functional>

#include "vector3d.h"
#include "constants.h"

////////////////////////////////////////
//            Declaration             //
////////////////////////////////////////

template <typename T>
struct field final
{
private:
    std::function<vector3d<T>(vector3d<T>)> f;

    field(std::function<vector3d<T>(vector3d<T>)> f);

public:
    void set_function(std::function<vector3d<T>(vector3d<T>)> f_new);

    vector3d<T> operator()(vector3d<T> r) const;

    field<T> operator=(const field<T> f);
    field<T> operator+(const field<T> f) const;
    field<T> operator-(const field<T> f) const;
    std::function<T(vector3d<T>)>
    operator*(const field<T> f) const;
    std::function<T(vector3d<T>)>
    operator*(const vector3d<T> v) const;
    field<T> operator*(const T d) const;
    field<T> operator^(const field<T> f) const;
    field<T> operator^(const vector3d<T> v) const;

    template <typename T_mult>
    friend field<T_mult> operator*(const T_mult d, const field<T_mult> f);

    template <typename T_mult>
    friend std::function<T_mult(vector3d<T_mult>)>
    operator*(const vector3d<T_mult> v, const field<T_mult> f);

    template <typename T_mult>
    friend field<T_mult> operator^(const vector3d<T_mult> d, const field<T_mult> f);

    template <typename T_cust>
    friend field<T_cust> new_field(std::function<vector3d<T_cust>(vector3d<T_cust>)> f);
};

////////////////////////////////////////
//             Definition             //
////////////////////////////////////////

template <typename T>
field<T>::field(std::function<vector3d<T>(vector3d<T>)> f) : f(f) {}

template <typename T>
void field<T>::set_function(std::function<vector3d<T>(vector3d<T>)> f_new)
{
    this->f = f_new;
    return;
}

template <typename T>
vector3d<T> field<T>::operator()(vector3d<T> r) const
{
    return f(r);
}

template <typename T>
field<T> field<T>::operator=(const field<T> f)
{
    this->f = f.f;
    return *this;
}

template <typename T>
field<T> field<T>::operator+(const field<T> f) const
{
    field<T> tmp([&, f](vector3d<T> r){
        return this->f(r) + f.operator()(r);
    });
    return tmp;
}

template <typename T>
field<T> field<T>::operator-(const field<T> f) const
{
    field<T> tmp([&, f](const vector3d<T> r){
        return this->f(r) - f.operator()(r);
    });
    return tmp;
}

template <typename T>
std::function<T(vector3d<T>)>
field<T>::operator*(const field<T> f) const
{
    return [&, f](vector3d<T> r){
        return this->operator()(r) * f.operator()(r);
    };
}

template <typename T>
std::function<T(vector3d<T>)>
field<T>::operator*(const vector3d<T> v) const
{
    return [&, v](vector3d<T> r){
        return this->operator()(r) * r;
    };
}

template <typename T>
std::function<T(vector3d<T>)>
operator*(const vector3d<T> v, const field<T> f)
{
    return [v, f](vector3d<T> r){
        return v * f.operator()(r);
    };
}

template <typename T>
field<T> field<T>::operator*(const T d) const
{
    field<T> tmp([&, d](vector3d<T> r){
        return d * this->operator()(r);
    });
    return tmp;
}

template <typename T>
field<T> operator*(const T d, const field<T> f)
{
    field<T> tmp([d, f](vector3d<T> r){
        return d * f.operator()(r);
    });
    return tmp;
}

template <typename T>
field<T> field<T>::operator^(const field<T> f) const
{
    field<T> tmp([&, f](vector3d<T> r){
        return this->operator()(r) ^ f.operator()(r);
    });
    return tmp;
}

template <typename T>
field<T> field<T>::operator^(const vector3d<T> v) const
{
    field<T> tmp([&, v](vector3d<T> r){
        return this->operator()(r) ^ v;
    });
    return tmp;
}

template <typename T>
field<T> operator^(const vector3d<T> v, const field<T> f)
{
    field<T> tmp([v, f](vector3d<T> r){
        return v ^ f.operator()(r);
    });
    return tmp;
}

template <typename T>
field<T> new_field(std::function<vector3d<T>(vector3d<T>)> f)
{
    field<T> tmp(f);
    return tmp;
}

template <typename T>
field<T> new_field(vector3d<T> r)
{
    return new_field<T>([r](vector3d<T> v){return r;});
}

template <typename T>
field<T> new_field()
{
    return new_field<T>(vector3d<T>(0,0,0));
}

template <typename T>
field<T> penning_E(T C, T eps)
{
    field<T> tmp = new_field<T>();
    tmp.set_function([C, eps](vector3d<T> r){
        return C * vector3d<T>(
            (1 + eps) * r.x(),
            (1 - eps) * r.y(),
            -2 * r.z());
    });
    return tmp;
}

template <typename T>
field<T> penning_M(T B, T theta, T phi)
{
    field<T> tmp = new_field<T>();
    tmp.set_function([B, theta, phi](vector3d<T> r){
        return B * vector3d<T>(
            sin(theta) * cos(phi),
            sin(theta) * cos(phi),
            cos(theta)
        );
    });
    return tmp;
}

template <typename T>
field<T> penning_M(vector3d<T> B)
{
    return new_field(B);
}

#endif // FIELD_H