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

static Communication::Wifi wifi; TaskHandle_t xHandle[4]; 
ElectroStimulation::bioSignalController **signal;
uint8_t levelPin[4] = {2, 16, 5, 19},
          modPin[4] = {15, 4, 17, 18};
adxl345 accel;

void wifiCallback(Communication::Wifi &wifi1)
{
	uint8_t ch = 0;
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = new ControlHandler::systemLoopHandler<uint32_t>();
    idStructure->pid = new ControlHandler::PID<uint32_t>("100,33,0");
    idStructure->pid->setLimits(60,0.0);
    idStructure->signal = new ElectroStimulation::bioSignalController;
    idStructure->signal->powerControllerInit((gpio_num_t) levelPin[ch], (adc1_channel_t) ch, 10000, 
                                           (ledc_channel_t)ch, (ledc_timer_t)ch);
    idStructure->boost = new ModelHandler::ARX<uint32_t>(1,1);
    idStructure->rls   = new OptimizationHandler::RecursiveLeastSquare<uint32_t>(idStructure->boost); 
    idStructure->startLoop(ControlHandler::systemExitationForIdentificationLoop);   

    xTaskCreatePinnedToCore(ControlHandler::squaredWaveExitationLoop, "squaredWaveExitationLoop", 8*1024, idStructure, 8, &xHandle[ch], 0);
    vTaskDelay( 10000 / portTICK_PERIOD_MS );
    vTaskDelete(xHandle[ch]);

    std::stringstream ss; ss << std::setw(2*5+1) << std::setprecision(5) << std::fixed << "\nEntrada | Saida \n";
    for(unsigned j = 0; j < 2000; ++j)
    {
        ss << idStructure->in[j] << "   " << idStructure->out[j] << "\n";
    }
    wifi << ss.str();
}



int64_t xx_time_get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL));
}


extern "C" void app_main()
{ 
    
    //accel.init();
    ////accel.calibrate();
    //signal = new ElectroStimulation::bioSignalController*[4]();
    wifi.connect();
    wifi >> wifiCallback;
    
    
    
    //uint32_t minLimit = 40, maxLimit = 55;
    //ModelHandler::ARX<uint32_t> boost(1,1);
    //LinAlg::Matrix<double> inOut(101,2); 
    //uint32_t *in = new uint32_t[2000], *out = new uint32_t[2000];
    
    
    //while(1){
        //ets_delay_us(1000000);
        //uint64_t t = esp_timer_get_time();
        //for(unsigned j = 0; j < 10; ++j)
       // {
        //    //identificacao.setPowerLevel(minLimit);
       //     for(unsigned i = 0; i < 100; ++i)
       //     {
       //         identificacao.setPowerLevel(pid.OutputControl(minLimit,identificacao.getFeedbackForPowerControl()));
        //    }
        //    //identificacao.setPowerLevel(maxLimit);
        //    for(unsigned i = 0; i < 100; ++i)
         //   {
         //       identificacao.setPowerLevel(pid.OutputControl(maxLimit,identificacao.getFeedbackForPowerControl()));
         //   }
        //}
        //t = esp_timer_get_time() - t;
        //for(unsigned j = 0; j < 200; ++j)
        //{
        //    rls.optimize(in[j], out[j]);
        //}
       
        //std::cout << "\ntempo ="<< ((float)t) << "\n primeiro out: " << out[0] << "\n";

    //}
}

// algumas considerações a serem realizadas. O tempo mínimo que consegui para leitura do ADC com o PID foi de 12,25us, e isso parece ser dependente do esp utilizado.