#ifndef FIELD_H
#define FIELD_H

#include <cassert>
#include <functional>
#include <string>
#include <type_traits>

#include "vector3d.h"
#include "constants.h"

////////////////////////////////////////
//            Declaration             //
////////////////////////////////////////

template <typename T>
struct field final
{
private:
    std::function<vector3d<T>(T, vector3d<T>)> f;

    field(std::function<vector3d<T>(T, vector3d<T>)>);

public:

    // Methods
    void set_function(std::function<vector3d<T>(T, vector3d<T>)>);
    void set_function(std::function<vector3d<T>(vector3d<T>, T)>);
    void set_function(std::function<vector3d<T>(vector3d<T>)>);
    void set_function(std::function<vector3d<T>(T)>);
    void set_function(std::function<vector3d<T>()>);

    vector3d<T> operator()(T, vector3d<T>) const;
    vector3d<T> operator()(vector3d<T>, T) const;
    vector3d<T> operator()(vector3d<T>) const;
    vector3d<T> operator()(T) const;
    vector3d<T> operator()() const;

    // Binary Operators
    field<T> operator=(const field<T>);
    field<T> operator+(const field<T>) const;
    field<T> operator-(const field<T>) const;
    std::function<T(T, vector3d<T>)>
    operator*(const field<T>) const;
    std::function<T(T, vector3d<T>)>
    operator*(const vector3d<T>) const;
    field<T> operator*(const T) const;
    field<T> operator/(const T) const;
    field<T> operator^(const field<T>) const;
    field<T> operator^(const vector3d<T>) const;

    template <typename T_mult>
    friend field<T_mult>
    operator*(const T_mult, const field<T_mult>);

    template <typename T_mult>
    friend std::function<T_mult(T, vector3d<T_mult>)>
    operator*(const vector3d<T_mult>, const field<T_mult>);

    template <typename T_mult>
    friend field<T_mult>
    operator^(const vector3d<T_mult>, const field<T_mult>);

    // Unary Operators
    field<T> operator+=(const field<T>);
    field<T> operator-=(const field<T>);
    field<T> operator^=(const field<T>);
    field<T> operator*=(const T);
    field<T> operator/=(const T);
    field<T> operator -() const;

    // Function for constructing a field
    template <typename T_cust>
    friend field<T_cust>
    new_field(std::function<vector3d<T_cust>(T_cust, vector3d<T_cust>)>);
};

////////////////////////////////////////
//             Definition             //
////////////////////////////////////////

////////////////////////////////////////
//            Constructor             //
////////////////////////////////////////

template <typename T>
field<T>::field(std::function<vector3d<T>(T, vector3d<T>)> f) : f(f) {}

////////////////////////////////////////
//              Methods               //
////////////////////////////////////////

template <typename T>
void field<T>::set_function(std::function<vector3d<T>(T, vector3d<T>)> f_new)
{
    this->f = f_new;
    return;
}

template <typename T>
void field<T>::set_function(std::function<vector3d<T>(vector3d<T>, T)> f_new)
{
    this->f = [f_new](T t, vector3d<T> r){
        return f_new(r, t);
    };
    return;
}

template <typename T>
void field<T>::set_function(std::function<vector3d<T>(vector3d<T>)> f_new)
{
    this->f = [f_new](T t, vector3d<T> r){
        return f_new(r);
    };
    return;
}

template <typename T>
void field<T>::set_function(std::function<vector3d<T>(T)> f_new)
{
    this->f = [f_new](T t, vector3d<T> r){
        return f_new(t);
    };
    return;
}

template <typename T>
void field<T>::set_function(std::function<vector3d<T>()> f_new)
{
    this->f = [f_new](T t, vector3d<T> r){
        return f_new();
    };
    return;
}

template <typename T>
vector3d<T> field<T>::operator()(T t, vector3d<T> r) const
{
    return f(t, r);
}

template <typename T>
vector3d<T> field<T>::operator()(vector3d<T> r, T t) const
{
    return f(t, r);
}

template <typename T>
vector3d<T> field<T>::operator()(vector3d<T> r) const
{
    return f(0, r);
}

template <typename T>
vector3d<T> field<T>::operator()(T t) const
{
    return f(t, 0);
}

template <typename T>
vector3d<T> field<T>::operator()() const
{
    return f(0, 0);
}

////////////////////////////////////////
//          Binary Operators          //
////////////////////////////////////////

template <typename T>
field<T> field<T>::operator=(const field<T> f)
{
    this->f = f.f;
    return *this;
}

template <typename T>
field<T> field<T>::operator+(const field<T> f) const
{
    field<T> tmp([&, f](T t, vector3d<T> r){
        return this->f(t, r) + f.operator()(t, r);
    });
    return tmp;
}

template <typename T>
field<T> field<T>::operator-(const field<T> f) const
{
    field<T> tmp([&, f](T t, vector3d<T> r){
        return this->f(t, r) - f.operator()(t, r);
    });
    return tmp;
}

template <typename T>
std::function<T(T, vector3d<T>)>
field<T>::operator*(const field<T> f) const
{
    return [&, f](T t, vector3d<T> r){
        return this->operator()(t, r) * f.operator()(t, r);
    };
}

template <typename T>
std::function<T(T, vector3d<T>)>
field<T>::operator*(const vector3d<T> v) const
{
    return [&, v](T t, vector3d<T> r){
        return this->operator()(t, r) * r;
    };
}

template <typename T>
std::function<T(T, vector3d<T>)>
operator*(const vector3d<T> v, const field<T> f)
{
    return [v, f](T t, vector3d<T> r){
        return v * f.operator()(t, r);
    };
}

template <typename T>
field<T> field<T>::operator*(const T d) const
{
    field<T> tmp([&, d](T t, vector3d<T> r){
        return d * this->operator()(t, r);
    });
    return tmp;
}

template <typename T>
field<T> operator*(const T d, const field<T> f)
{
    field<T> tmp([d, f](T t, vector3d<T> r){
        return d * f.operator()(t, r);
    });
    return tmp;
}

template<typename T>
field<T> field<T>::operator/(const T d) const
{
    field<T> tmp([&, d](T t, vector3d<T> r){
        return this->operator()(t, r) / d;
    });
    return tmp;
}

template <typename T>
field<T> field<T>::operator^(const field<T> f) const
{
    field<T> tmp([&, f](T t, vector3d<T> r){
        return this->operator()(t, r) ^ f.operator()(t, r);
    });
    return tmp;
}

template <typename T>
field<T> field<T>::operator^(const vector3d<T> v) const
{
    field<T> tmp([&, v](T t, vector3d<T> r){
        return this->operator()(t, r) ^ v;
    });
    return tmp;
}

template <typename T>
field<T> operator^(const vector3d<T> v, const field<T> f)
{
    field<T> tmp([v, f](T t, vector3d<T> r){
        return v ^ f.operator()(t, r);
    });
    return tmp;
}

////////////////////////////////////////
//          Unary Operators           //
////////////////////////////////////////

template <typename T>
field<T> field<T>::operator+=(const field<T> F)
{
    std::function<vector3d<T>(T, vector3d<T>)> tmp = this->f;
    this->set_function([tmp, F](T t, vector3d<T> r){
        tmp(r) + F.operator()(t, r);
    });
    return *this;
}

template <typename T>
field<T> field<T>::operator-=(const field<T> F)
{
    std::function<vector3d<T>(T, vector3d<T>)> tmp = this->f;
    this->set_function([tmp, F](T t, vector3d<T> r){
        tmp(r) - F.operator()(t, r);
    });
    return *this;
}

template <typename T>
field<T> field<T>::operator^=(const field<T> F)
{
    std::function<vector3d<T>(T, vector3d<T>)> tmp = this->f;
    this->set_function([tmp, F](T t, vector3d<T> r){
        tmp(r) ^ F.operator()(t, r);
    });
    return *this;
}

template <typename T>
field<T> field<T>::operator*=(const T mult)
{
    std::function<vector3d<T>(T, vector3d<T>)> tmp = this->f;
    this->set_function([tmp, mult](T t, vector3d<T> r){
        tmp(t, r) * mult;
    });
    return *this;
}

template <typename T>
field<T> field<T>::operator/=(const T mult)
{
    std::function<vector3d<T>(T, vector3d<T>)> tmp = this->f;
    this->set_function([tmp, mult](T t, vector3d<T> r){
        tmp(t, r) / mult;
    });
    return *this;
}

template <typename T>
field<T> field<T>::operator-() const
{
    field<T> tmp([&](T t, vector3d<T> r){
        return -this->operator()(t, r);
    });
    return tmp;
}

////////////////////////////////////////
//      Functions that construct      //
//            new fields              //
////////////////////////////////////////

template <typename T>
field<T> new_field(std::function<vector3d<T>(T, vector3d<T>)> f)
{
    field<T> tmp(f);
    return tmp;
}

template <typename T>
field<T> new_field(std::function<vector3d<T>(vector3d<T>, T)> f)
{
    return new_field<T>([f](T t, vector3d<T> r){
        return f(r, t);
    });
}

template <typename T>
field<T> new_field(std::function<vector3d<T>(vector3d<T>)> f)
{
    return new_field<T>([f](T t, vector3d<T> r){
        return f(r);
    });
}

template <typename T>
field<T> new_field(std::function<vector3d<T>(T)> f)
{
    return new_field<T>([f](T t, vector3d<T> r){
        return f(t);
    });
}

template <typename T>
field<T> new_field(std::function<vector3d<T>()> f)
{
    return new_field<T>([f](T t, vector3d<T> r){
        return f();
    });
}

template <typename T>
field<T> new_field(vector3d<T> r)
{
    return new_field<T>([r](T t, vector3d<T> v){return r;});
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
    tmp.set_function([C, eps](T t, vector3d<T> r){
        return C * vector3d<T>(
            (1 + eps) * r.x(),
            (1 - eps) * r.y(),
            -2 * r.z());
    });
    return tmp;
}

template <typename T>
field<T> penning_M(vector3d<T> B)
{
    return new_field(B);
}

#endif // FIELD_H