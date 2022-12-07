#ifndef SOLVING_METHODS_H
#define SOLVING_METHODS_H

#include <array>
#include <string>
#include <functional>

////////////////////////////////////////
//            Declaration             //
////////////////////////////////////////

template<class T, typename S>
T heun(
    S t,
    T phase,
    std::function<T(S,T)> f,
    S h
);

template<class T, typename S>
T rk45(
    S t,
    T phase,
    std::function<T(S,T)> f,
    S h
);

/*template<class T, typename S>
T simple_step(
    S t,
    T phase,
    std::function<narray<T>(S,narray<T>)> f,
    S h,
    std::string type
);*/

template<class T, typename S>
T step(
    S t,
    T phase,
    std::function<T(S,T)> f,
    S h,
    std::string type,
    unsigned long long N
);

////////////////////////////////////////
//            Definition              //
////////////////////////////////////////

template<class T, typename S>
T heun(
    S t,
    T phase,
    std::function<T(S,T)> f,
    S h
)
{
    T k = phase + h * f(t, phase);
    return phase + .5 * h * (f(t, phase) + f(t + h, k));
}

template<class T, typename S>
T rk45(
    S t,
    T phase,
    std::function<T(S,T)> f,
    S h
)
{
    T k1 = f(t, phase);
    T k2 = f(t + .5 * h, phase + .5 * h * k1);
    T k3 = f(t + .5 * h, phase + .5 * h * k2);
    T k4 = f(t + h, phase + h * k3);
    return phase + (h / 6) * (k1 + 2 * k2 + 2 * k3 + k4);
}

/*template<class T, typename S>
T simple_step(
    S t,
    T phase,
    std::function<narray<T>(S,narray<T>)> f,
    S h,
    std::string type = "rk45"
)
{
    if (type == "heun")
        return heun(t, phase, f, h);
    if (type == "rk45")
        return rk45(t, phase, f, h);
}*/

template<class T, typename S>
T step(
    S t,
    T phase,
    std::function<T(S,T)> f,
    S h,
    std::string type,
    unsigned N
)
{
    T k = phase;
    if (type == "heun")
        for(unsigned i = 0; i < N; ++i)
            k = heun(t, k, f, h);
    if (type == "rk45")
        for(unsigned i = 0; i < N; ++i)
            k = rk45(t, k, f, h);

    return k;
}

#endif // SOLVING_METHODS_h