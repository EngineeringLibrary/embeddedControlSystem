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

ControlHandler::systemLoopHandler<long double> *idStructure = new ControlHandler::systemLoopHandler<long double>();
uint8_t levelPin[1] = {2},
        modPin[2]   = {15, 4};
bool flag;

void wifiCallback(Communication::Wifi &wifi1)
{
    LinAlg::Matrix<long double> code = wifi1.getData();
	std::cout << code;

    idStructure->channel = code(0,0); uint8_t cmd = code(0,1); 
    idStructure->pid[idStructure->channel] = new ControlHandler::PID<long double>();
    idStructure->pid[idStructure->channel]->setLimits(code(0,2),code(0,3));
    idStructure->pid[idStructure->channel]->setParams(code(0,4),code(0,5),code(0,6));//kp,pi,kd
    idStructure->pid[idStructure->channel]->setSampleTime(12.5);
    idStructure->controllerSensibility = code(0,7);
    idStructure->controllerReference = code(0,8);
    idStructure->operationalInput    = code(0,9);
    idStructure->operationalOutput   = code(0,10);
    idStructure->controller = code(0,11);
    idStructure->tuningMethod = code(0,12);

    idStructure->relayReference = code(0,13);
    idStructure->relayMinLimit  = code(0,14);
    idStructure->relayMaxLimit  = code(0,15);
    idStructure->tolerance = code(0,16);
    idStructure->reactionCurveReference = code(0,17); 

    idStructure->signal[idStructure->channel] = new ElectroStimulation::bioSignalController;
    idStructure->signal[idStructure->channel]->addSignalBehavior("freq",    code(0,18));
    idStructure->signal[idStructure->channel]->addSignalBehavior("period",  code(0,19));
    idStructure->signal[idStructure->channel]->addSignalBehavior("ccLevel", code(0,20));
    idStructure->maxIterator = code(0,21);

    idStructure->signal[idStructure->channel]->powerControllerInit((gpio_num_t) levelPin[idStructure->channel], (adc1_channel_t) idStructure->channel, 10000, 
                                (ledc_channel_t)idStructure->channel, (ledc_timer_t)idStructure->channel);
    idStructure->signal[idStructure->channel]->setOutputHandlerDirectPin((gpio_num_t) modPin[idStructure->channel]);
    idStructure->signal[idStructure->channel]->setOutputHandlerReversePin((gpio_num_t) modPin[idStructure->channel+1]);

    idStructure->xHandle[idStructure->channel] = new TaskHandle_t;
    idStructure->boost[idStructure->channel]   = new ModelHandler::ARX<long double>(1,1); idStructure->boost[idStructure->channel]->setSampleTime(12.5);
    idStructure->rls[idStructure->channel]     = new OptimizationHandler::RecursiveLeastSquare<long double>(idStructure->boost[idStructure->channel]);

    switch(cmd){
        case 0: xTaskCreatePinnedToCore(ControlHandler::relayExitationLoop,                 "CLNC",        8*1024, idStructure,             8, &idStructure->xHandle[idStructure->channel], 0); break;
        case 1: xTaskCreatePinnedToCore(ControlHandler::squaredWaveExitationLoop,           "CLNC",        8*1024, idStructure,             8, &idStructure->xHandle[idStructure->channel], 0); break;
        //case 0: xTaskCreatePinnedToCore(ElectroStimulation::burstController,                "burst",       4*1024, idStructure->signal[ch], 8, &idStructure->xHandle[ch], 1); break;
        //case 1: xTaskCreatePinnedToCore(ElectroStimulation::twoFaseNormalController,        "normal",      4*1024, idStructure->signal[ch], 8, &idStructure->xHandle[ch], 1); break;
        //case 2: xTaskCreatePinnedToCore(ElectroStimulation::modulationController,           "modulation",  4*1024, idStructure->signal[ch], 8, &idStructure->xHandle[ch], 1); break;
        //case 3: xTaskCreatePinnedToCore(ElectroStimulation::sd1Controller,                  "sd1",         4*1024, idStructure->signal[ch], 8, &idStructure->xHandle[ch], 1); break;
        //case 4: xTaskCreatePinnedToCore(ElectroStimulation::sd2Controller,                  "sd2",         4*1024, idStructure->signal[ch], 8, &idStructure->xHandle[ch], 1); break;
        //case 5: xTaskCreatePinnedToCore(ControlHandler::squaredWaveExitationLoop,           "squaredWave", 8*1024, idStructure,             8, &idStructure->xHandle[ch], 0); break;
        //case 6: xTaskCreatePinnedToCore(ControlHandler::relayExitationLoop,                 "relay",       8*1024, idStructure,             8, &idStructure->xHandle[ch], 0); break;
        //case 7: xTaskCreatePinnedToCore(ControlHandler::closedLoopTwoFaseNormalController,  "CLNC",        8*1024, idStructure,             8, &idStructure->xHandle[ch], 0); break;
        //case 8: xTaskCreatePinnedToCore(ControlHandler::relayExitationLoop, "relay", 8*1024, idStructure, 8, &idStructure->xHandle[ch], 0); break;
    }
        
   
        
	//}

    //accel.read();
    //std::cout << "Entrou 2\n";
    //std::stringstream ss; ss << accel.get_x();
    //wifi << ss.str();
}

extern "C" void app_main()
{ 
    idStructure->signal  = new ElectroStimulation::bioSignalController*[1]();
    idStructure->signal[0] = NULL; //idStructure->signal[1] = NULL; idStructure->signal[1] = NULL; idStructure->signal[1] = NULL;
    idStructure->xHandle = new TaskHandle_t[1];
    idStructure->xHandle[0] = NULL; //idStructure->xHandle[1] = NULL; idStructure->xHandle[1] = NULL; idStructure->xHandle[1] = NULL;
    idStructure->boost   = new ModelHandler::ARX<long double>*[1];
    idStructure->pid     = new ControlHandler::PID<long double>*[1];
    idStructure->rls     = new OptimizationHandler::RecursiveLeastSquare<long double>*[1];
    idStructure->wifi.connect();
    idStructure->wifi >> wifiCallback;
    vTaskStartScheduler();	

    // ModelHandler::ARX<double> arx(1,1); 
    // std::cout << "Entrou 1" << std::endl;
    // arx.setModelCoef("0.949896, 0.51987");
    // std::cout << "Entrou 2" << std::endl;
    // arx.setSampleTime(12.5);
    // std::cout << "Entrou 3" << std::endl;
    // LinAlg::Matrix<long double> FOP = c2dConversion(arx);
    // std::cout << FOP << std::endl;
    // for(uint_fast8_t i = 0; i < 10; ++i){
    //     ControlHandler::PID<long double> pid = ControlHandler::controllerTuning(FOP,"PI", ControlHandler::tune[i]);
    //     std::cout << pid.getParams() << std::endl;
    // }
    // while(1);
}