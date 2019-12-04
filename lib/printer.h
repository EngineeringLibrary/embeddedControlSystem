#ifndef PRINTER_H
#define PRINTER_H
#include <string>
#include "headers/comunication/comunication.h"
#include "headers/control/control.h"
#include "headers/model/model.h"
#include "headers/motor/motor.h"
#include "headers/optimizer/optmizer.h"
#include "headers/sensor/sensor.h"

namespace PrinterHandler {
    template<typename Type>
    class Printer
    {
    public:
        Printer();
        void stop   ();
        void start  ();
        void pause  ();
        void resume ();
        void calibration();
        void saveCalibration();
        void loadCalibration();
        void comandInterpretation(std::string comand);

    private:
        std::deque<OptimizationHandler::Optmizer<Type>*> opt;
        Comunication *com;

        std::deque<ModelHandler::Model<Type>*> model;
        std::deque<Motor*>   motors;
        std::deque<Sensor*>  sensors;
        std::deque<Control*> controllers;
        Storage *storage;
    };
}

#endif // PRINTER_H
