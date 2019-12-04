#include <iostream>
#include <iomanip>
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
uint8_t levelPin[4] = {2, 16, 5, 19},
          modPin[4] = {15, 4, 17, 18};
adxl345 accel;

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

	if(signal[ch]){
		vTaskDelete(xHandle[ch]);
	    delete signal[ch];
        signal[ch] = NULL;
	}
	
	if(cmd)
	{
        // uint8_t mod = msg[2];
	    // uint16_t freq = (msg[4] || (msg[3] << 8));
	    // uint16_t period = (msg[6] || (msg[5] << 8));
        // uint8_t pwr = msg[7];

        signal[ch] = new ElectroStimulation::bioSignalController;
        signal[ch]->powerControllerInit((gpio_num_t) levelPin[ch], (adc1_channel_t) ch, 10000, 
                                        (ledc_channel_t)ch, (ledc_timer_t)ch);
        signal[ch]->setOutputHandlerDirectPin((gpio_num_t) modPin[ch]);
        signal[ch]->addSignalBehavior("freq", freq);
        signal[ch]->addSignalBehavior("period", period);
        signal[ch]->addSignalBehavior("ccLevel", pwr);
		
        switch(mod){
            case 0: xTaskCreatePinnedToCore(ElectroStimulation::burstController, "burst", 4*1024, signal[ch], 8, &xHandle[ch], 1); break;
            case 1: xTaskCreatePinnedToCore(ElectroStimulation::openLoopNormalController, "normal", 4*1024, signal[ch], 8, &xHandle[ch], 1); break;
            case 2: xTaskCreatePinnedToCore(ElectroStimulation::modulationController, "modulation", 4*1024, signal[ch], 8, &xHandle[ch], 1); break;
            case 3: xTaskCreatePinnedToCore(ElectroStimulation::sd1Controller, "sd1", 4*1024, signal[ch], 8, &xHandle[ch], 1); break;
            case 4: xTaskCreatePinnedToCore(ElectroStimulation::sd2Controller, "sd2", 4*1024, signal[ch], 8, &xHandle[ch], 1); break;
        }
	}
    accel.read();
    std::stringstream ss; ss << std::setw(2*5+1) << std::setprecision(5) << std::fixed 
    << accel.get_filtered_x() << ",  " << accel.get_pitch() << ",  " << accel.get_roll();
    wifi << ss.str();
}

extern "C" void app_main()
{ 
    accel.init();
    //accel.calibrate();
    signal = new ElectroStimulation::bioSignalController*[4]();
    wifi.connect();
    wifi >> wifiCallback;
}