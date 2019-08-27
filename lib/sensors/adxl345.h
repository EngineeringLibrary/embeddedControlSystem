#ifndef ADRONE_ADXL345_H
#define ADRONE_ADXL345_H

#include <stdint.h>
#include "i2cHandler.h"
#include "iostream"

class adxl345
{
public:
    // setup registers
    bool init();
    // check wire connection
    bool connected();
    // read data from sensor
    void read();
    // calibrate sensor
    void calibrate();
    // get Y axie G aceleration
    const double& get_x() const;
    // get X axie G aceleration
    const double& get_y() const;
    // get Z axie G aceleration
    const double& get_z() const;

private:
    int16_t  _raw[3];     // raw data
    double   _data[3];    // aceleration data
    int16_t  _offset[3];  // offset data
};


#endif // ADRONE_ADXL345_H
