
#ifndef LAB6_POINT_H
#define LAB6_POINT_H
#include <iostream>
#include <algorithm>
#include "cmath"

template<class T>
struct point {
    T x;
    T y;
    double VectNorm(point l, point r) const;
    double VectProd(point l, point r) const;
};

template<class T>
std::istream& operator>> (std::istream& is, point<T>& p) {
    is >> p.x >> p.y;
    return is;
}

template<class T>
std::ostream& operator<< (std::ostream& os, const point<T>& p) {
    os << p.x << ' ' << p.y;
    return os;
}
template<class T>
point<T> operator-(const point<T> l, const point<T> r) {
    point<T> res;
    res.x = r.x - l.x;
    res.y = r.y - l.y;
    return res;
}
template<class T>
double VectProd(point<T> l, point<T> r) {
    return std::abs(l.x * r.y - r.x * l.y);
}

template<class T>
double VectNorm(point<T> l, point<T> r) {
    point<T> vect = operator-(l,r);
    double res = sqrt(vect.x* vect.x + vect.y * vect.y);
    return res;
}
#endif //LAB6_POINT_H
