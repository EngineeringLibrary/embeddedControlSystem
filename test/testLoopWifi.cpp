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


ControlHandler::systemLoopHandler<uint32_t> *idStructure = new ControlHandler::systemLoopHandler<uint32_t>();
uint8_t levelPin[4] = {2, 16, 5, 19},
        modPin[4]   = {15, 4, 17, 18};
bool flag;

void wifiCallback(Communication::Wifi &wifi1)
{
	std::string msg = wifi1.getData();
    uint8_t cmd = msg[0] - 1;
	uint8_t ch = msg[1] - 1; 

    uint8_t mod = msg[2] - 1;
    uint16_t freq = (((msg[3] - 1) << 8) | (msg[4] - 1));
    uint16_t period = (((msg[5] - 1) << 8) | (msg[6] - 1));
    uint8_t pwr = msg[7] - 1;
	
    std::cout << (uint16_t)cmd << "," << (uint16_t)ch << "," << (uint16_t)mod << "," << (uint16_t)freq << "," << (uint16_t)period << "," << (uint16_t)pwr << "\n";

	if(idStructure->signal[ch] != NULL /*&&!cmd*/){
        std::cout << "EntrouS 1\n";
	    delete idStructure->signal[ch];
        std::cout << "EntrouS 3\n";
        idStructure->signal[ch] = NULL;
        std::cout << "EntrouS 4\n";
        flag = false;
	}
	
	if(cmd)
	{
        if(!flag){
            idStructure->channel = ch;
            idStructure->minLimit = freq;
            idStructure->maxLimit = period;
            idStructure->maxIterator = 1500;
            idStructure->in = new uint32_t[idStructure->maxIterator]; idStructure->out = new uint32_t[idStructure->maxIterator];
            idStructure->signal[ch] = new ElectroStimulation::bioSignalController;
            idStructure->signal[ch]->powerControllerInit((gpio_num_t) levelPin[ch], (adc1_channel_t) ch, 10000, 
                                        (ledc_channel_t)ch, (ledc_timer_t)ch);
            idStructure->signal[ch]->setOutputHandlerDirectPin((gpio_num_t) modPin[ch]);
            idStructure->signal[ch]->addSignalBehavior("freq", freq);
            idStructure->signal[ch]->addSignalBehavior("period", period);
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

            flag = true;
            
        }
     
        
		
        switch(mod){
            case 0: xTaskCreatePinnedToCore(ControlHandler::squaredWaveExitationLoop, "squaredWave", 8*1024, idStructure, 8, &idStructure->xHandle[ch], 0); break;
            case 1: xTaskCreatePinnedToCore(ControlHandler::relayExitationLoop, "relay", 8*1024, idStructure, 8, &idStructure->xHandle[ch], 0); break;
        }
	}

    //accel.read();
    //std::cout << "Entrou 2\n";
    //std::stringstream ss; ss << accel.get_x();
    //wifi << ss.str();
}

extern "C" void app_main()
{ 
    idStructure->signal  = new ElectroStimulation::bioSignalController*[4]();
    idStructure->signal[0] = NULL; idStructure->signal[1] = NULL; idStructure->signal[2] = NULL; idStructure->signal[3] = NULL;
    idStructure->xHandle = new TaskHandle_t[4];
    idStructure->xHandle[0] = NULL; idStructure->xHandle[1] = NULL; idStructure->xHandle[2] = NULL; idStructure->xHandle[3] = NULL;
    idStructure->boost   = new ModelHandler::ARX<double>*[4];
    idStructure->pid     = new ControlHandler::PID<uint32_t>*[4];
    idStructure->rls     = new OptimizationHandler::RecursiveLeastSquare<double>*[4];
    idStructure->wifi.connect();
    idStructure->wifi >> wifiCallback;
    vTaskStartScheduler();	
}