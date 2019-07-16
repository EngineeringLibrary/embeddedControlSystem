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

Communication::Wifi wifi;
static void wifiTest(void*)
{
    std::string teste;
    while(1){
        wifi >> teste;
        // wifi << teste;
        // wifi << "\n";
    }
}
extern "C" void app_main()
{
    wifi.connect();
    xTaskCreate(&wifiTest, "Wifi Test", 5*1024, NULL, 5, NULL);
    // wifi.connect(signalParametersChange);
}