
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
#include "adxl345.h"
#include "pid.h"

adxl345 accel;
Communication::Serial uart(115200);
ControlHandler::PID<double> pid("1.0,1.0,0");
ElectroStimulation::bioSignalController *systemToBeControlled;
TaskHandle_t systemToBeControlled_xHandle;

extern "C" void app_main()
{
    // signal = new ElectroStimulation::bioSignalController*[10]();
    // wifi.connect();
    // wifi >> wifiCallback;
    
    accel.init();
    std::string valorRecebidoSerial;
    uart >> valorRecebidoSerial;
    std::stringstream ss(valorRecebidoSerial);
    double reference, u = 0.0;
    ss >> reference;
    pid.setLimits(100,0);

    systemToBeControlled = new ElectroStimulation::bioSignalController;
    systemToBeControlled->powerControllerInit((gpio_num_t)15, 50000, (ledc_channel_t)0);
    systemToBeControlled->setOutputHandlerPin((gpio_num_t)2);
    systemToBeControlled->addSignalBehavior("freq", 40);
    systemToBeControlled->addSignalBehavior("period", 300);
    systemToBeControlled->addSignalBehavior("ccLevel", 0);
    xTaskCreatePinnedToCore(ElectroStimulation::normalController, "normal", 2 * 1024, systemToBeControlled, 8, &systemToBeControlled_xHandle, 1);
    while (1)
    {
        accel.read();
        u = pid.OutputControl(reference,accel.get_x());

        vTaskDelete(systemToBeControlled_xHandle);
        delete systemToBeControlled;
        systemToBeControlled = NULL;
        systemToBeControlled = new ElectroStimulation::bioSignalController;
        systemToBeControlled->powerControllerInit((gpio_num_t)15, 50000, (ledc_channel_t)0);
        systemToBeControlled->setOutputHandlerPin((gpio_num_t)2);
        systemToBeControlled->addSignalBehavior("freq", 40);
        systemToBeControlled->addSignalBehavior("period", 300);
        systemToBeControlled->addSignalBehavior("ccLevel", u);
        xTaskCreatePinnedToCore(ElectroStimulation::normalController, "normal", 2 * 1024, systemToBeControlled, 8, &systemToBeControlled_xHandle, 1);
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}