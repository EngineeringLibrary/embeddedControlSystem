#include <iostream>
#include <iomanip>
#include <time.h>
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "driver/ledc.h"
#include "bioSignalGenerator.h"
#include "wifi/wifi.h"
#include "serial.h"
#include <time.h>
//#include "../test/test.h"
#include "adxl345.h"
#include "systemLoop.h"

ControlHandler::systemLoopHandler<uint32_t> *idStructure = new ControlHandler::systemLoopHandler<uint32_t>();
uint8_t levelPin[4] = {2, 16, 5, 19},
          modPin[4] = {15, 4, 17, 18};
adxl345 accel;

void wifiCallback(Communication::Wifi &wifi1)
{
    uint8_t ch = 0;
	std::cout << "entrou 2\n";
    idStructure->pid = new ControlHandler::PID<uint32_t>("100,33,0");
    std::cout << "entrou 3\n";
    idStructure->pid->setLimits(60,0.0);
    std::cout << "entrou 4\n";
    idStructure->signal = new ElectroStimulation::bioSignalController;
    std::cout << "entrou 5\n";
    idStructure->signal->powerControllerInit((gpio_num_t) levelPin[ch], (adc1_channel_t) ch, 10000, 
                                           (ledc_channel_t)ch, (ledc_timer_t)ch);
                                           std::cout << "entrou 1\n";
    idStructure->boost = new ModelHandler::ARX<uint32_t>(1,1);
    std::cout << "entrou 6\n";
    idStructure->rls   = new OptimizationHandler::RecursiveLeastSquare<uint32_t>(idStructure->boost);
    std::cout << "entrou 7\n";
    idStructure->xHandle = new TaskHandle_t; 
    std::cout << "entrou 8\n";
    //vTaskDelay( 30000 / portTICK_PERIOD_MS );
    idStructure->startLoop(ControlHandler::systemExitationForIdentificationLoop); 

    std::cout << "entrou 9\n";
    xTaskCreatePinnedToCore(ControlHandler::squaredWaveExitationLoop, "squaredWaveExitationLoop", 8*1024, 
                            idStructure, 8, idStructure->xHandle, 0);
}

extern "C" void app_main()
{ 
    //accel.init();
    ////accel.calibrate();
    //signal = new ElectroStimulation::bioSignalController*[4]();
    
    idStructure->wifi.connect();
    std::cout << "entrou 1\n";
    idStructure->wifi >> wifiCallback;
    
    while(1);
}