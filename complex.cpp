#include "complex.h"

Complex::Complex(double a, double b):
    m_A(a),
    m_B(b)
{
}

Complex Complex::toExpComplex(const Complex &x)
{
    Complex c;
    c.m_A=pow(pow(x.m_A,2)+pow(x.m_B,2),0.5);
    if(isAlmostZero(x.m_A) && isAlmostZero(x.m_B)){
        c.m_B=double(0);//无意义
    }
    else if(isAlmostZero(x.m_A) && !isAlmostZero(x.m_B)){
        if(x.m_B>0)c.m_B=double(MY_PI/double(2));
        else c.m_B=double((MY_PI/double(2))*double(3));
    }
    else if(!isAlmostZero(x.m_A) && isAlmostZero(x.m_B)){
        if(x.m_A>0)c.m_B=double(0);
        else c.m_B=double(MY_PI);
    }
    else {
        if(x.m_A>0 && x.m_B>0)
            c.m_B=atan(fabs(x.m_B/x.m_A))+double(0);
        else if(x.m_A<0 && x.m_B>0)
            c.m_B=atan(fabs(x.m_A/x.m_B))+double(MY_PI/double(2));
        else if(x.m_A<0 && x.m_B<0)
            c.m_B=atan(fabs(x.m_B/x.m_A))+double(MY_PI);
        else if(x.m_A>0 && x.m_B<0)
            c.m_B=atan(fabs(x.m_A/x.m_B))+double((MY_PI/double(2))*double(3));
    }
    return c;
}

Complex Complex::toDescartesComplex(const Complex &x)
{
    Complex c;
    c.m_A=x.m_A*cos(x.m_B);
    c.m_B=x.m_A*sin(x.m_B);
    return c;
}

