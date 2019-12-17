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
#include "systemLoop.h"

ControlHandler::systemLoopHandler<double> *idStructure = new ControlHandler::systemLoopHandler<double>();
uint8_t levelPin[1] = {2},
        modPin[2]   = {15, 4};
bool flag = false;

void wifiCallback(Communication::Wifi &wifi1)
{
    if(idStructure->xHandle[idStructure->channel] != NULL && flag) vTaskDelete(idStructure->xHandle[idStructure->channel]);
    flag = true;
    LinAlg::Matrix<double> code = wifi1.getData();
	std::cout << code;

    idStructure->channel = 0; uint8_t cmd = code(0,0); 
    idStructure->pid[idStructure->channel] = new ControlHandler::PID<double>();
    idStructure->pid[idStructure->channel]->setLimits(code(0,1),code(0,2));
    idStructure->pid[idStructure->channel]->setParams(code(0,3),code(0,4),code(0,5));//kp,pi,kd
    idStructure->pid[idStructure->channel]->setSampleTime(1);
    idStructure->controllerSensibility = code(0,6);
    idStructure->controllerReference = code(0,7);

    idStructure->signal[idStructure->channel] = new ElectroStimulation::bioSignalController;
    idStructure->signal[idStructure->channel]->addSignalBehavior("freq",    code(0,8));
    idStructure->signal[idStructure->channel]->addSignalBehavior("period",  code(0,9));
    idStructure->signal[idStructure->channel]->addSignalBehavior("ccLevel", code(0,10));
    idStructure->maxIterator = code(0,11);

    idStructure->signal[idStructure->channel]->powerControllerInit((gpio_num_t) levelPin[idStructure->channel], (adc1_channel_t) idStructure->channel, 10000, 
                                (ledc_channel_t)idStructure->channel, (ledc_timer_t)idStructure->channel);
    idStructure->signal[idStructure->channel]->setOutputHandlerDirectPin((gpio_num_t) modPin[idStructure->channel]);
    idStructure->signal[idStructure->channel]->setOutputHandlerReversePin((gpio_num_t) modPin[idStructure->channel+1]);

    idStructure->xHandle[idStructure->channel] = new TaskHandle_t;
    idStructure->boost[idStructure->channel]   = new ModelHandler::ARX<double>(1,1); idStructure->boost[idStructure->channel]->setSampleTime(12.5);
    idStructure->rls[idStructure->channel]     = new OptimizationHandler::RecursiveLeastSquare<double>(idStructure->boost[idStructure->channel]);

    switch(cmd){
        case 0: xTaskCreatePinnedToCore(ControlHandler::LimiarTest,         "CLNC",8*1024, idStructure,8, &idStructure->xHandle[idStructure->channel], 1); break;
        case 1: xTaskCreatePinnedToCore(ControlHandler::angleControlNormal, "CLNC",8*1024, idStructure,8, &idStructure->xHandle[idStructure->channel], 0); break;
        case 2: if(idStructure->xHandle[idStructure->channel] != NULL) vTaskDelete(idStructure->xHandle[idStructure->channel]);
    }
        
}

extern "C" void app_main()
{ 
    idStructure->signal  = new ElectroStimulation::bioSignalController*[1]();
    idStructure->signal[0] = NULL; //idStructure->signal[1] = NULL; idStructure->signal[1] = NULL; idStructure->signal[1] = NULL;
    idStructure->xHandle = new TaskHandle_t[1];
    idStructure->xHandle[0] = NULL; //idStructure->xHandle[1] = NULL; idStructure->xHandle[1] = NULL; idStructure->xHandle[1] = NULL;
    idStructure->boost   = new ModelHandler::ARX<double>*[1];
    idStructure->pid     = new ControlHandler::PID<double>*[1];
    idStructure->rls     = new OptimizationHandler::RecursiveLeastSquare<double>*[1];
    idStructure->accel.init(); 
    idStructure->wifi.connect();
    idStructure->wifi >> wifiCallback;
}