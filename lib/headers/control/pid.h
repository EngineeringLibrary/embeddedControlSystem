#ifndef PID_H
#define PID_H
#include "control.h"

class PID : public Control
{
public:
    PID();
    double outputControlActionCalculation(double reference, double input);//vai virar matrix
    std::string print();
};

#endif // PID_H
