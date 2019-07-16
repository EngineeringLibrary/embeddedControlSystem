#ifndef TEST_H
#define TEST_H
#include <iostream>
#include <time.h>
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bioSignalGenerator.h"
#include "wifi/wifi.h"
#include "serial.h"

namespace Test{
    static ElectroStimulation::bioSignalController signal1, signal2, signal3, signal4, signal5, signal6;

    void sd2Test(){
        signal1.powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
        signal1.setOutputHandlerPin((gpio_num_t) 12);
        signal1.addSignalBehavior("freq", 1);
        signal1.addSignalBehavior("period", 100);
        signal1.addSignalBehavior("ccLevel", 100);

        xTaskCreate(ElectroStimulation::sd2Controller, "sd2", 2048, &signal1, 1, NULL);
    }

    void sd1Test(){
        signal2.powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
        signal2.setOutputHandlerPin((gpio_num_t) 12);
        signal2.addSignalBehavior("freq", 1);
        signal2.addSignalBehavior("period", 100);
        signal2.addSignalBehavior("ccLevel", 100);

        xTaskCreate(ElectroStimulation::sd1Controller, "sd1", 2048, &signal2, 1, NULL);
    }

    void modulationTest(){
        signal3.powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
        signal3.setOutputHandlerPin((gpio_num_t) 12);
        signal3.addSignalBehavior("freq", 1);
        signal3.addSignalBehavior("period", 100);
        signal3.addSignalBehavior("ccLevel", 100);

        xTaskCreate(ElectroStimulation::modulationController, "modulation", 2048, &signal3, 1, NULL);
    }

    void normalTest(){
        signal4.powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
        signal4.setOutputHandlerPin((gpio_num_t) 12);
        signal4.addSignalBehavior("freq", 1);
        signal4.addSignalBehavior("period", 100);
        signal4.addSignalBehavior("ccLevel", 100);

        xTaskCreate(ElectroStimulation::normalController, "normal", 2048, &signal4, 1, NULL);
    }

    void burstTest(){
        signal5.powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
        signal5.setOutputHandlerPin((gpio_num_t) 12);
        signal5.addSignalBehavior("freq", 1);
        signal5.addSignalBehavior("period", 100);
        signal5.addSignalBehavior("ccLevel", 100);

        xTaskCreate(ElectroStimulation::burstController, "burst", 2048, &signal5, 1, NULL);
    }

    void MarchaTest(){
        signal6.powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
        signal6.setOutputHandlerPin((gpio_num_t) 12);
        signal6.addSignalBehavior("freq", 1);
        signal6.addSignalBehavior("period", 100);
        signal6.addSignalBehavior("ccLevel", 100);

        xTaskCreate(ElectroStimulation::burstController, "burst", 2048, &signal6, 1, NULL);
    }

    void signalParametersChange(std::string data)
    {
        data = data.substr(0, data.find("\r"));
        std::cout << data;
        signal5.addSignalBehavior("freq", data[0]);
        signal5.addSignalBehavior("period", data[1]);
        signal5.addSignalBehavior("ccLevel", data[2]);

    }

    Communication::Serial uart(1500);
    void seriaTest()
    {
        uart.connect();
        std::string teste;
        while(1){
            uart >> teste;
            uart << teste;
            uart << "\n";
        }
    }
}

#endif