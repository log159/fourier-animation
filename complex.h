#ifndef COMPLEX_H
#define COMPLEX_H

#define MY_PI acos(-1)

#include <cmath>

class Complex
{
public:
    double m_A;//实部或模长
    double m_B;//虚部或弧度角
public:
    Complex(double a=0,double b=0);

    static Complex toExpComplex(const Complex& x);

    static Complex toDescartesComplex(const Complex& x);

    static bool isAlmostZero(double value,  double epsilon = 1e-9)noexcept
    {
        return std::fabs(value) < epsilon;
    }

};

#endif // COMPLEX_H
