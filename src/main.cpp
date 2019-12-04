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
#include "conversions.h"
#include "pidTuning.h"

ControlHandler::systemLoopHandler<uint32_t> *idStructure = new ControlHandler::systemLoopHandler<uint32_t>();
uint8_t levelPin[1] = {2},
        modPin[2]   = {15, 4};
bool flag;

void wifiCallback(Communication::Wifi &wifi1)
{
	std::string msg = wifi1.getData();
    bool cmd = msg[0] - 1;
	uint8_t ch = 0;//msg[1] - 1; 

    uint8_t mod = msg[2] - 1;
    uint16_t freq = (((msg[3] - 1) << 8) | (msg[4] - 1));
    uint16_t period = (((msg[5] - 1) << 8) | (msg[6] - 1));
    uint8_t pwr = msg[7] - 1;
	
    std::cout << (uint16_t)cmd << "," << (uint16_t)ch << "," << (uint16_t)mod << "," << (uint16_t)freq << "," << (uint16_t)period << "," << (uint16_t)pwr << "\n";
	
	//if(cmd)
	//{
        // if(idStructure->signal[ch] != NULL && idStructure->xHandle[ch] != NULL){
        //     vTaskDelete(idStructure->xHandle[ch]);
        //     std::cout << "EntrouS 1\n";
        //     delete idStructure->signal[ch];
        //     std::cout << "EntrouS 3\n";
        //     idStructure->signal[ch] = NULL;
        //     std::cout << "EntrouS 4\n";
        //     flag = false;
	    // }
        idStructure->channel  = ch;
        idStructure->minLimit = freq;
        idStructure->maxLimit = period;
        idStructure->tolerance = cmd;

        //idStructure->maxIterator = 1500;
        //idStructure->in = new uint32_t[idStructure->maxIterator]; idStructure->out = new uint32_t[idStructure->maxIterator];
        idStructure->signal[ch] = new ElectroStimulation::bioSignalController;
        idStructure->signal[ch]->powerControllerInit((gpio_num_t) levelPin[ch], (adc1_channel_t) ch, 10000, 
                                    (ledc_channel_t)ch, (ledc_timer_t)ch);
        idStructure->signal[ch]->setOutputHandlerDirectPin((gpio_num_t) modPin[ch]);
        idStructure->signal[ch]->setOutputHandlerReversePin((gpio_num_t) modPin[ch+1]);
        //idStructure->signal[ch]->addSignalBehavior("freq", freq);
        //idStructure->signal[ch]->addSignalBehavior("period", period);
        idStructure->signal[ch]->addSignalBehavior("ccLevel", pwr);
        std::cout << "Entrou 1\n";

        idStructure->xHandle[ch] = new TaskHandle_t;
        std::cout << "Entrou 2\n";
        idStructure->boost[ch]   = new ModelHandler::ARX<double>(1,1);
        std::cout << "Entrou 3\n";
        idStructure->pid[ch]     = new ControlHandler::PID<uint32_t>("100,33,0");
        std::cout << "Entrou 4\n";
        idStructure->rls[ch]     = new OptimizationHandler::RecursiveLeastSquare<double>(idStructure->boost[ch]);
        
        std::cout << "Entrou 5\n";
        //xTaskCreatePinnedToCore(ControlHandler::squaredWaveExitationLoop, "squaredWaveExitationLoop", 8*1024, 
        //                idStructure, 8, idStructure->xHandle[ch], 0);
        // flag = true;

        switch(mod){
            case 0: xTaskCreatePinnedToCore(ControlHandler::relayExitationLoop,                 "CLNC",        8*1024, idStructure,             8, &idStructure->xHandle[ch], 0); break;
            case 1: xTaskCreatePinnedToCore(ControlHandler::squaredWaveExitationLoop,           "CLNC",        8*1024, idStructure,             8, &idStructure->xHandle[ch], 0); break;
            case 2: xTaskCreatePinnedToCore(ControlHandler::closedLoopTwoFaseNormalController,  "CLNC",        8*1024, idStructure,             8, &idStructure->xHandle[ch], 0); break;
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
    // idStructure->signal  = new ElectroStimulation::bioSignalController*[1]();
    // idStructure->signal[0] = NULL; //idStructure->signal[1] = NULL; idStructure->signal[1] = NULL; idStructure->signal[1] = NULL;
    // idStructure->xHandle = new TaskHandle_t[1];
    // idStructure->xHandle[0] = NULL; //idStructure->xHandle[1] = NULL; idStructure->xHandle[1] = NULL; idStructure->xHandle[1] = NULL;
    // idStructure->boost   = new ModelHandler::ARX<double>*[1];
    // idStructure->pid     = new ControlHandler::PID<uint32_t>*[1];
    // idStructure->rls     = new OptimizationHandler::RecursiveLeastSquare<double>*[1];
    // idStructure->wifi.connect();
    // idStructure->wifi >> wifiCallback;
    // vTaskStartScheduler();	

    ModelHandler::ARX<double> arx(1,1); 
    std::cout << "Entrou 1" << std::endl;
    arx.setModelCoef("0.949896, 0.51987");
    std::cout << "Entrou 2" << std::endl;
    arx.setSampleTime(12.5);
    std::cout << "Entrou 3" << std::endl;
    LinAlg::Matrix<long double> FOP = c2dConversion(arx);
    std::cout << FOP << std::endl;
    for(uint_fast8_t i = 0; i < 10; ++i){
        ControlHandler::PID<long double> pid = ControlHandler::controllerTuning(FOP,"PI", ControlHandler::tune[i]);
        std::cout << pid.getParams() << std::endl;
    }
    while(1);
}