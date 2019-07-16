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
#include "C:/Users/andre.dantas/Documents/embeddedControlSystem/test/test.h"

static Communication::Wifi wifi; TaskHandle_t xHandleBurst; static bool entrou = 0;
ElectroStimulation::bioSignalController *signal;

void wifiCallback(Communication::Wifi &wifi1)
{
    unsigned char test = wifi1.getData()[0];
    std::cout << test << "\n";
    if(!entrou){
        signal = new ElectroStimulation::bioSignalController;
        signal->powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
        signal->setOutputHandlerPin((gpio_num_t) 12);
        signal->addSignalBehavior("freq", 1);
        signal->addSignalBehavior("period", 100);
        signal->addSignalBehavior("ccLevel", (uint8_t) test);
        entrou = !entrou;

        xTaskCreatePinnedToCore(ElectroStimulation::burstController, "burst", 2*1024, signal, 8, &xHandleBurst, 1);
    }
    else
    {
        vTaskDelete(xHandleBurst);
        std::cout << "Deletou a task" << "\n";
        delete signal;
        signal = new ElectroStimulation::bioSignalController;
        ets_delay_us(100);
        signal->powerControllerInit((gpio_num_t) 5, 50000, (ledc_channel_t)0);
        signal->setOutputHandlerPin((gpio_num_t) 12);
        signal->addSignalBehavior("freq", 1);
        signal->addSignalBehavior("period", 100);
        signal->addSignalBehavior("ccLevel", (uint8_t) test);

        xTaskCreatePinnedToCore(ElectroStimulation::burstController, "modulation", 2048, signal, 8, &xHandleBurst, 1);
        // entrou = !entrou;
        // ::xTaskCreatePinnedToCore(&ElectroStimulation::burstController, "burst", 2*1024, &Test::signal5, 8, &xHandleBurst, 1);
    }

}

void wifiTest()
{
    wifi.connect();
    wifi >> wifiCallback;
}
extern "C" void app_main()
{
    // xTaskCreate(&wifiTest, "Wifi Test", 5*1024, NULL, 4, NULL);
    wifiTest();
    // wifi.connect(signalParametersChange);
}