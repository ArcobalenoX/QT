#include "complex.h"
#include "math.h"
complex::complex(float r,float i)
{
    R=r ;
    I=i ;
}


void complex :: setR(float r){
    R= r;
}
void complex :: setI(float i){
    I=i;
}

 float complex :: getR(void)const{
    return R;
}
float complex :: getI(void)const{
    return I;
}
float complex :: getModulo(void){
    return sqrt(R*R+I*I);
}

complex complex :: operator + (const complex& c){
    complex ret;
    ret.setR(this->getR()+c.getR());
    ret.setI(this->getI()+c.getI());
    return ret;
}
complex complex :: operator - (const complex& c){
    complex ret;
    ret.setR(this->getR()-c.getR());
    ret.setI(this->getI()-c.getI());
    return ret;
}
complex complex :: operator * (const complex& c){
    float r = R*c.R-I*c.I;
    float i = R*c.R+I*c.I;
    return complex(r,i);
}
complex complex :: operator / (const complex& c){
    float der= c.R*c.R+c.I*c.I;
    float r = (R*c.R+I*c.I)/der;
    float i = (I*c.R-R*c.I)/der;
    return complex(r,i);
}

complex complex :: operator = (const complex& c)
{
    R=c.getR();    //this->setR(c.getR());
    I=c.getI();    //this->setI(c.getI());
    return *this;
}
bool complex :: operator == (const complex& c)
{
    return (R==c.R && I==c.I);
}
bool complex :: operator != (const complex& c)
{
    return !(*this==c);
}
/*
complex a(1,2);
complex b(2,3);
cout<<"R "<<(a*b).getR()<<endl;
cout<<"I "<<(a*b).getI()<<endl;
complex c;
c=(a+b);
c=a=b;
cout<<(a!=b)<<endl;
cout<<c.getModulo()<<endl;
*/



