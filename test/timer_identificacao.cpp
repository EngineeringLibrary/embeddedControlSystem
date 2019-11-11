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
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

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
        ets_delay_us(100);
		
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



int64_t xx_time_get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL));
}

#define TIMER_INTR_SEL TIMER_INTR_LEVEL  /*!< Timer level interrupt */
#define TIMER_GROUP    TIMER_GROUP_0     /*!< Test on timer group 0 */
#define TIMER_DIVIDER   80000               /*!< Hardware timer clock divider, 80 to get 1MHz clock to timer */
#define TIMER_SCALE    (TIMER_BASE_CLK / TIMER_DIVIDER)  /*!< used to calculate counter value */
#define TIMER_FINE_ADJ   (0*(TIMER_BASE_CLK / TIMER_DIVIDER)/1000000) /*!< used to compensate alarm value */
#define TIMER_INTERVAL0_SEC   (0.001)   /*!< test interval for timer 0 */

struct identification_t{
    identification_t(){}
    ControlHandler::PID<uint32_t> pid;
    ElectroStimulation::bioSignalController signal;
    OptimizationHandler::RecursiveLeastSquare<uint32_t> rls;
    ModelHandler::ARX<uint32_t> *boost;
    timer_group_t timer_group;
    timer_idx_t timer_idx;
    timer_config_t config;
    uint32_t counter;
};

void IRAM_ATTR timer_isr(void *para){// timer group 0, ISR
    //identification_t idStructure = *((identification_t*) para);
    //timer_idx_t timer_idx = idStructure.timer_idx;
    uint32_t intr_status = TIMERG0.int_st_timers.val;
      if((intr_status & BIT(TIMER_0))) {
          TIMERG0.hw_timer[TIMER_0].update = 1;
          TIMERG0.int_clr_timers.t0 = 1;
          TIMERG0.hw_timer[TIMER_0].config.alarm_en = 1;
          //idStructure.pid.OutputControl(0,idStructure.signal.getFeedbackForPowerControl());
          //pidStructure.signal.setPowerLevel();
          //++idStructure.counter;
          //idStructure.signal.setPowerLevel(idStructure.counter>>2);
          //gpio_set_level(GPIO_NUM_16,idStructure.counter%2);
      }
     // std::cout << idStructure.signal.getFeedbackForPowerControl() << std::endl;
      std::cout << "0\n";
}

static void tg0_timer0_init()
{
    timer_group_t timer_group;
    timer_idx_t timer_idx;
    timer_config_t config;
    timer_idx = TIMER_0;
    timer_group = TIMER_GROUP_0;
    config.alarm_en = 1;
    config.auto_reload = 1;
    config.counter_dir = TIMER_COUNT_UP;
    config.divider = TIMER_DIVIDER;
    config.intr_type = TIMER_INTR_SEL;
    config.counter_en = TIMER_PAUSE;
    /*Configure timer*/
    timer_init(timer_group, timer_idx, &config);
    /*Stop timer counter*/
    timer_pause(timer_group, timer_idx);
    /*Load counter value */
    timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);
    /*Set alarm value*/
    timer_set_alarm_value(timer_group, timer_idx, (TIMER_INTERVAL0_SEC * TIMER_SCALE) - TIMER_FINE_ADJ);
    /*Enable timer interrupt*/
    timer_enable_intr(timer_group, timer_idx);
    /*Set ISR handler*/
    timer_isr_register(timer_group, timer_idx, timer_isr, (void*) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
    /*Start timer counter*/
    timer_start(timer_group, timer_idx);
}


extern "C" void app_main()
{ 
    
    //accel.init();
    ////accel.calibrate();
    //signal = new ElectroStimulation::bioSignalController*[4]();
    //wifi.connect();
    //wifi >> wifiCallback;

    uint8_t ch = 0;
    identification_t idStructure;
    idStructure.counter = 0;
    idStructure.timer_idx = TIMER_0;
    idStructure.timer_group = TIMER_GROUP_0;
    idStructure.pid.setParams("100,33,0");
    idStructure.pid.setLimits(60.0,0.0);
    
    idStructure.signal.powerControllerInit((gpio_num_t) levelPin[ch], (adc1_channel_t) ch, 10000, 
                                        (ledc_channel_t)ch, (ledc_timer_t)ch);
    // gpio_set_direction(GPIO_NUM_2,GPIO_MODE_OUTPUT);
    idStructure.boost = new ModelHandler::ARX<uint32_t>(1,1);
    idStructure.rls.initialization(idStructure.boost); 
                            
    
    //uint32_t minLimit = 40, maxLimit = 55;
    //ModelHandler::ARX<uint32_t> boost(1,1);
    //LinAlg::Matrix<double> inOut(101,2); 
    //uint32_t *in = new uint32_t[2000], *out = new uint32_t[2000];
    tg0_timer0_init();
    
    while(1){
        ets_delay_us(1000000);
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

    }
}