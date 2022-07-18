#ifndef COMPLEX_H
#define COMPLEX_H


class complex
{
private:
    float R;
    float I;

public:
    complex(float r=0,float i=0);

    void setR(float r);
    void setI(float i);
    float getR(void)const;
    float getI(void) const;
    float getModulo(void);
    complex operator + (const complex& c);
    complex operator - (const complex& c);
    complex operator = (const complex& c);
    complex operator * (const complex& c);
    complex operator / (const complex& c);
    bool operator == (const complex& c);
    bool operator != (const complex& c);
};

#endif // COMPLEX_H
