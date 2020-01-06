
#ifndef LAB6_TRAPEZE_H
#define LAB6_TRAPEZE_H
#include "point.h"
template <class T>
struct trapeze {
private:
    point<T> a_,b_,c_,d_;
public:
    point<T> center() const;
    void print(std::ostream& os) const ;
    double area() const;
    trapeze(std::istream& is);
};

template<class T>
double trapeze<T>::area() const {
    if ( (VectProd(operator-(a_,b_), operator-(c_,d_)) == 0) && (VectProd(operator-(b_,c_), operator-(a_,d_)) == 0) ) {
        return fabs((VectProd(operator-(a_,b_), operator-(a_,d_)))) ;
    } else if (VectProd(operator-(a_,b_), operator-(d_,c_)) == 0) {
        return ((VectNorm(a_, b_) + VectNorm(d_, c_)) / 2) * sqrt(
                VectNorm(d_, a_) * VectNorm(d_, a_) - (
                        pow((
                                    (pow((VectNorm(d_, c_) - VectNorm(a_, b_)), 2) +
                                     VectNorm(d_, a_) * VectNorm(d_, a_) - VectNorm(b_, c_) * VectNorm(b_, c_)) /
                                    (2 * (VectNorm(d_, c_) - VectNorm(a_, b_)))
                            ), 2)
                )
        );

    } else if (VectProd(operator-(b_,c_), operator-(a_,d_)) == 0) {
        return ((VectNorm(b_, c_) + VectNorm(a_, d_)) / 2) * sqrt(
                VectNorm(a_, b_) * VectNorm(a_, b_) - (
                        pow((
                                    (pow((VectNorm(a_, d_) - VectNorm(b_, c_)), 2) +
                                     VectNorm(a_, b_) * VectNorm(a_, b_) - VectNorm(c_, d_) * VectNorm(c_, d_)) /
                                    (2 * (VectNorm(a_, d_) - VectNorm(b_, c_)))
                            ), 2)
                )
        );
    }
}

template<class T>
trapeze<T>::trapeze(std::istream& is) {
    is >> a_ >> b_ >> c_ >> d_ ;
}

template<class T>
void trapeze<T>::print(std::ostream& os) const {
    os << "coordinate:\n"<<"\n"<< a_ << '\n' << b_ << '\n' << c_ << '\n' << d_ << '\n';
}

template<class T>
point<T> trapeze<T>::center() const {
    T x,y;
    x = (a_.x + b_.x + c_.x + d_.x ) / 4;
    y = (a_.y + b_.y + c_.y + d_.y) / 4;
    return {x,y};
}

#endif //LAB6_TRAPEZE_H
