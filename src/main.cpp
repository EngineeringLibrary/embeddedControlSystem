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
//#include "../test/test.h"
#include "adxl345.h"

static Communication::Wifi wifi; TaskHandle_t xHandle[4]; 
ElectroStimulation::bioSignalController **signal;
uint8_t levelPin[10] = {2, 0, 16, 5, 19, 22, 32, 25, 27, 12},
          modPin[10] = {15, 4, 17, 18, 21, 23, 33, 26, 14, 13};
adxl345 accel; bool flag;

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

	if(signal[ch]&&!cmd){
		vTaskDelete(xHandle[ch]);
	    delete signal[ch];
        signal[ch] = NULL;
	}
	
	if(cmd)
	{
        std::cout << "Entrou antes\n";
        if(!flag){
            std::cout << "Entrou 1\n";
            signal[ch] = new ElectroStimulation::bioSignalController;
            std::cout << "Entrou 2\n";
            signal[ch]->powerControllerInit((gpio_num_t) levelPin[ch], (adc1_channel_t) ch, 10000, 
                                        (ledc_channel_t)ch, (ledc_timer_t)ch);
            signal[ch]->setOutputHandlerDirectPin((gpio_num_t) modPin[ch]);
            signal[ch]->addSignalBehavior("freq", freq);
            signal[ch]->addSignalBehavior("period", period);
            signal[ch]->addSignalBehavior("ccLevel", pwr);
            std::cout << "Entrou 3\n";
            flag = 1;
        }
        std::cout << "Entrou 4\n";
        signal[ch]->setPowerLevel(pwr);
        std::cout << "Entrou 5\n";
        
		
        //switch(mod){
        //    case 0: xTaskCreatePinnedToCore(ElectroStimulation::burstController, "burst", 2*1024, signal[ch], 8, &xHandle[ch], 1); break;
        //    case 1: xTaskCreatePinnedToCore(ElectroStimulation::openLoopNormalController, "normal", 2*1024, signal[ch], 8, &xHandle[ch], 1); break;
        //    case 2: xTaskCreatePinnedToCore(ElectroStimulation::modulationController, "modulation", 2*1024, signal[ch], 8, &xHandle[ch], 1); break;
        //    case 3: xTaskCreatePinnedToCore(ElectroStimulation::sd1Controller, "sd1", 2*1024, signal[ch], 8, &xHandle[ch], 1); break;
        //    case 4: xTaskCreatePinnedToCore(ElectroStimulation::sd2Controller, "sd2", 2*1024, signal[ch], 8, &xHandle[ch], 1); break;
        //}
	}
    std::cout << "saiu\n";
    //accel.read();
    //std::cout << "Entrou 2\n";
    //std::stringstream ss; ss << accel.get_x();
    //wifi << ss.str();
}

extern "C" void app_main()
{ 
    accel.init();
    signal = new ElectroStimulation::bioSignalController*[4]();
    wifi.connect();
    wifi >> wifiCallback;
}