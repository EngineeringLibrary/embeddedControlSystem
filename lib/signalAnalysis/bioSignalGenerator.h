#ifndef BIOSIGNALGENERATOR_H
#define BIOSIGNALGENERATOR_H

#include <iostream>
#include <time.h>
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include "pid.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "map"

namespace ElectroStimulation{
    class bioSignalController
    {
    public:
        bioSignalController(){}
        void powerControllerInit(const gpio_num_t &pin, const adc1_channel_t &feedbackPin, const uint32_t &freq, const ledc_channel_t &channel, const ledc_timer_t &timer);
        void setPowerLevel(const double &powerLevel);
        void setOutputHandlerPin(const gpio_num_t &outputHandlerPin);
        gpio_num_t getOutputHandlerPin () const {return outputHandlerPin;}

        void addSignalBehavior(const std::string &signalBehaviorName, const double &signalBehavior);
        void removeSignalBehavior(const std::string &signalBehaviorName);
        double getSignalBehavior(const std::string &signalBehavior) const;
        double getFeedbackForPowerControl() {return 1.149*adc1_get_raw(this->feedbackPin)/4096;}

    private:

        ledc_channel_config_t ledc_channel;
        ledc_timer_config_t ledc_timer;
        gpio_num_t outputHandlerPin; adc1_channel_t feedbackPin;
        std::map<std::string, double> signalBehaviorHandler;
    };

    static void burstController(void*);
    static void normalController(void*);
    static void modulationController(void*);
    static void sd1Controller(void*);
    static void sd2Controller(void*);
}

#include "bioSignalGenerator.hpp"
#endif // BIOSIGNALGENERATOR_H
