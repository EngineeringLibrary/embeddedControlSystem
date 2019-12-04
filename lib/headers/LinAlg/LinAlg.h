#ifndef OPTMIZER_H
#define OPTMIZER_H


class Optmizer
{
public:
    Optmizer();
    virtual void   optimize();

    virtual void   setmodel();
    virtual void   setConstraints();

    virtual void   getmodel();
    virtual double getConstraints();
    virtual double getOptimizedValue();

    virtual void   print();
protected:
    double optimizedValue, constraints;
    Model model;
};

#endif // OPTMIZER_H
