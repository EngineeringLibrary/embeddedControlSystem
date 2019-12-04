#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
public:
    Motor();
    void setConversionFactor(double absolutePosition){this->absolutePosition = absolutePosition;}
    void setAbsolutePosition(double absolutePosition){this->absolutePosition = absolutePosition;}

    virtual void setPosition(double position) = 0;
    template <typename Type>
    virtual LinAlg::Matrix<Type> getLimits  (gpio_num_t maxLimitSensor, gpio_num_t minLimitSensor, gpio_num_t positionSensor) = 0;

    void setSampleTime(double sampleTime){this->sampleTime = sampleTime;}
    unsigned int getPosition(){return this->position;}
    double getSampleTime(){return this->sampleTime;}
    double getAbsolutePosition(){return this->absolutePosition;}
    double getConversionFactor(){return this->absolutePosition;}

protected:
    double sampleTime, absolutePosition, conversionFactor; //Fator de conversao para unidades de engenharia
    double minPositionLimit, maxPositionLimit, minSpeedLimit, maxSpeedLimit;
    unsigned int position;
};

#endif // MOTOR_H
