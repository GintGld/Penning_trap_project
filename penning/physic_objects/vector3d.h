#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <string>
#include <cmath>

////////////////////////////////////////
//            Declaration             //
////////////////////////////////////////

template<typename T>
class vector3d final
{
protected:
    T* data;
public:
    // Constructors
    vector3d();
    vector3d(const T& x, const T& y, const T& z);
    vector3d(T* arr);

    // RAII
    vector3d(const vector3d& lhs);
    vector3d& operator=(const vector3d& lhs);
    vector3d(vector3d&& rhs);
    vector3d& operator=(vector3d&& rhs);
    ~vector3d();

    // Binary operators
    bool      operator==(const vector3d& other) const;
    bool      operator!=(const vector3d& other) const;
    vector3d& operator+ (const vector3d& other) const;
    vector3d& operator- (const vector3d& other) const;
    T&        operator* (const vector3d& other) const;
    vector3d& operator^ (const vector3d& other) const;
    vector3d& operator* (const T& mult) const;
    vector3d& operator/ (const T& mult) const;

    // Unary operators
    vector3d& operator+=(const vector3d& other);
    vector3d& operator-=(const vector3d& other);
    vector3d& operator*=(const T& mult);
    vector3d& operator/=(const T& mult);
    vector3d& operator^=(const vector3d& other);
    vector3d& operator- () const;

    // Methods
    T& x();
    T& y();
    T& z();
    T  x() const;
    T  y() const;
    T  z() const;
    T  len() const;
    void print();
    template<typename T_for_ostream>
    friend std::ostream & operator << (std::ostream& os, const vector3d<T_for_ostream>& v);
    template<typename T1, typename T2>
    friend vector3d<T2>& operator*(const T1& m, const vector3d<T2>& v);
    template<typename T_for_string>
    friend std::string to_str(const vector3d<T_for_string>& v);
};

#define vector_f  vector3d<float>
#define vector_d  vector3d<double>
#define vector_ld vector3d<long double>

////////////////////////////////////////
//             Definition             //
////////////////////////////////////////

////////////////////////////////////////
//            Constructors            //
////////////////////////////////////////
template<typename T>
vector3d<T>::vector3d():
    data(new T[3]) {}

template<typename T>
vector3d<T>::vector3d(const T& x_, const T& y_, const T& z_):
    data(new T[3])
{
    data[0] = x_;
    data[1] = y_;
    data[2] = z_;
}

template<typename T>
vector3d<T>::vector3d(T* arr):
    data(arr) {}

////////////////////////////////////////
//                RAII                //
////////////////////////////////////////
template<typename T>
vector3d<T>::vector3d(const vector3d<T>& lhs):
    data(new T[3])
{
    data[0] = lhs.data[0];
    data[1] = lhs.data[1];
    data[2] = lhs.data[2];
}

template<typename T>
vector3d<T>& vector3d<T>::operator=(const vector3d& lhs)
{
    if (this == &lhs)
        return *this;
    vector3d<T> tmp(lhs);
    std::swap(data, tmp.data);
    return *this;
}

template<typename T>
vector3d<T>::vector3d(vector3d<T>&& rhs):
    data(rhs.data)
{
    rhs.data = nullptr;
}

template<typename T>
vector3d<T>& vector3d<T>::operator=(vector3d&& rhs)
{
    vector3d<T> tmp(std::move(rhs));
    std::swap(data, tmp.data);
    return *this;
}

template<typename T>
vector3d<T>::~vector3d()
{
    delete[] data;
}

////////////////////////////////////////
//          Binary Operators          //
////////////////////////////////////////
template<typename T>
bool vector3d<T>::operator==(const vector3d<T>& other) const
{
    bool f = 1;
    f &= (data[0] == other.data[0]);
    f &= (data[1] == other.data[1]);
    f &= (data[2] == other.data[2]);
    return f;
}

template<typename T>
bool vector3d<T>::operator!=(const vector3d<T>& other) const
{
    return !(this->operator==(other));
}

template<typename T>
vector3d<T>& vector3d<T>::operator+(const vector3d<T>& other) const
{
    vector3d<T>* tmp = new vector3d<T>(data[0]+other.data[0], data[1]+other.data[1], data[2]+other.data[2]);
    return *tmp;
}

template<typename T>
vector3d<T>& vector3d<T>::operator-(const vector3d<T>& other) const
{
    vector3d<T>* tmp = new vector3d<T>(data[0]-other.data[0], data[1]-other.data[1], data[2]-other.data[2]);
    return *tmp;
}

template<typename T>
T& vector3d<T>::operator*(const vector3d<T>& other) const
{
    T* res = new T;
    *res = data[0] * other.data[0] + data[1] * other.data[1] + data[2] * other.data[2];
    return *res;
}

template<typename T>
vector3d<T>& vector3d<T>::operator^(const vector3d<T>& other) const
{
    vector3d<T>* tmp = new vector3d<T>;
    tmp->data[0] = data[1] * other.data[2] - data[2] * other.data[1];
    tmp->data[1] = data[2] * other.data[0] - data[0] * other.data[2];
    tmp->data[2] = data[0] * other.data[1] - data[1] * other.data[0];
    return *tmp;
}

template<typename T>
vector3d<T>& vector3d<T>::operator*(const T& mult) const
{
    vector3d<T>* tmp = new vector3d<T>(data[0] * mult, data[1] * mult, data[2] * mult);
    return *tmp;
}

template<typename T1, typename T2>
vector3d<T2>& operator*(const T1& m, const vector3d<T2>& v)
{
    vector3d<T2>* tmp = new vector3d<T2>(v.x() * m, v.y() * m, v.z() * m);
    return *tmp;
}

template<typename T>
vector3d<T>& vector3d<T>::operator/(const T& d) const
{
    vector3d<T>* tmp = new vector3d<T>(this->x()/d, this->y()/d, this->z()/d);
    return *tmp;
}

////////////////////////////////////////
//           Unary operators          //
////////////////////////////////////////
template<typename T>
vector3d<T>& vector3d<T>::operator +=(const vector3d& other)
{
    data[0] += other.data[0];
    data[1] += other.data[1];
    data[2] += other.data[2];
    return *this;
}

template<typename T>
vector3d<T>& vector3d<T>::operator -=(const vector3d& other)
{
    data[0] -= other.data[0];
    data[1] -= other.data[1];
    data[2] -= other.data[2];
    return *this;
}

template<typename T>
vector3d<T>& vector3d<T>::operator *=(const T& mult)
{
    data[0] *= mult;
    data[1] *= mult;
    data[2] *= mult;
    return *this;
}

template<typename T>
vector3d<T>& vector3d<T>::operator/=(const T& mult)
{
    data[0] /= mult;
    data[1] /= mult;
    data[2] /= mult;
    return *this;
}

template<typename T>
vector3d<T>& vector3d<T>::operator ^=(const vector3d& other)
{
    vector3d<T> tmp;
    tmp.data[0] = data[1] * other.data[2] - data[2] * other.data[1];
    tmp.data[1] = data[2] * other.data[0] - data[0] * other.data[2];
    tmp.data[2] = data[0] * other.data[1] - data[1] * other.data[0];
    this->operator=(tmp);
    return *this;
}

template<typename T>
vector3d<T>& vector3d<T>::operator -() const
{
    vector3d<T>* tmp = new vector3d<T>(-data[0], -data[1], -data[2]);
    return *tmp;
}

////////////////////////////////////////
//              Methods               //
////////////////////////////////////////
template<typename T>
T& vector3d<T>::x()
{
    return data[0];
}

template<typename T>
T& vector3d<T>::y()
{
    return data[1];
}

template<typename T>
T& vector3d<T>::z()
{
    return data[2];
}

template<typename T>
T vector3d<T>::x() const
{
    return data[0];
}

template<typename T>
T vector3d<T>::y() const
{
    return data[1];
}

template<typename T>
T vector3d<T>::z() const
{
    return data[2];
}

template<typename T>
T vector3d<T>::len() const
{
    return sqrt(this->operator*(*this));
}

template<typename T>
void vector3d<T>::print()
{
    std::cout << "(" << data[0] << ", " << data[1] << ", " << data[2] << ")\n";
    return;
}

template<typename T>
std::ostream & operator <<(std::ostream& os, const vector3d<T>& v)
{
    return os << "(" << v.data[0] << ", " << v.data[1] << ", " << v.data[2] << ")\n"; 
}

template<typename T>
std::string to_str(const vector3d<T>& v)
{
    return "(" + std::to_string(v.data[0]) + ", " + std::to_string(v.data[1]) + ", " + std::to_string(v.data[2]) + ")"; 
}

#endif // VECTOR3D_H