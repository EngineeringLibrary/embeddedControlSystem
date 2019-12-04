#ifndef CONTROL_H
#define CONTROL_H
#include <string>

class Control // Vai virar template
{
public:
    Control();
    void setLimits(double limits); //vai virar matrix
    void setReference(double reference);//vai virar matrix
    void setSampleTime(double sampleTime);//vai virar matrix
    virtual double outputControlActionCalculation(double reference, double input) = 0;//vai virar matrix

    void getLimits(double limits); //vai virar matrix
    void getReference(double reference);//vai virar matrix
    void getSampleTime(double sampleTime);//vai virar matrix
    virtual std::string print();

protected:
    double outputControlAction, input, limits, sampleTime, transportDelay, reference;//vai virar matrix
};

template<typename Type>
std::ostream& operator<< (std::ostream& output, Control controller);
template<typename Type>
std::string& operator<< (std::string& output, const Control &controller);
#endif // CONTROL_H
