#include <iostream>
#include <time.h>
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bioSignalGenerator.h"

ElectroStimulation::bioSignalController signal1;

extern "C" void app_main()
{

    signal1.powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
    signal1.setOutputHandlerPin((gpio_num_t) 12);
    signal1.addSignalBehavior("Ton",150);
    signal1.addSignalBehavior("period",10000);

    xTaskCreate(ElectroStimulation::burstController, "burst", 2048, &signal1, 1, NULL);

}