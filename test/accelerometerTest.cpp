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

adxl345 accel;
extern "C" void app_main()
{
    // signal = new ElectroStimulation::bioSignalController*[10]();
    // wifi.connect();
    // wifi >> wifiCallback;
    accel.init();
    while (1)
    {
        accel.read();
        std::cout << accel.get_x() << std::endl;
        std::cout << accel.get_y() << std::endl;
        std::cout << accel.get_z() << std::endl;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}