#include <iostream>
#include <iomanip>
#include <time.h>
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi/wifi.h"

static Communication::Wifi wifi; 
gpio_num_t pin; ledc_channel_t channel; ledc_timer_t timer; ledc_channel_config_t ledc_channel;
ledc_timer_config_t ledc_timer;

void wifiCallback(Communication::Wifi &wifi1)
{
	std::string msg = wifi1.getData();
    uint8_t cmd = msg[0] - 1;
	uint8_t ch = msg[1] - 1; 

    uint8_t mod = msg[2] - 1;
    uint32_t freq = (((msg[3] - 1) << 8) | (msg[4] - 1));
    uint16_t period = (((msg[5] - 1) << 8) | (msg[6] - 1));
    uint8_t pwr = msg[7] - 1;
	
    std::cout << (uint16_t)cmd << "," << (uint16_t)ch << "," << (uint16_t)mod << "," << (uint16_t)freq << "," << (uint16_t)period << "," << (uint16_t)pwr << "\n";


    ledc_channel.duty       = (uint16_t)((pwr)*1024/100); 
    ledc_channel.channel    = channel;

    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.freq_hz = freq;
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num = (ledc_timer_t)1;
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
    ledc_timer_rst(ledc_timer.speed_mode, ledc_channel.timer_sel);

    vTaskDelay(1 / portTICK_RATE_MS);

    std::cout << ledc_get_freq(ledc_channel.speed_mode, ledc_channel.timer_sel) << std::endl;
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, ledc_channel.duty);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}

extern "C" void app_main()
{ 
    
    
    gpio_pad_select_gpio((gpio_num_t)2);
    gpio_set_direction((gpio_num_t)2, GPIO_MODE_OUTPUT); 

    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.freq_hz = 5000;
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num = (ledc_timer_t)1;
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    ledc_channel.duty       = 0; 
    ledc_channel.channel    = ledc_channel_t(0);
    ledc_channel.gpio_num   = (gpio_num_t)2;
    ledc_channel.timer_sel  = (ledc_timer_t)1;
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel_config(&ledc_channel);

    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, ledc_channel.duty);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);

    wifi.connect();
    wifi >> wifiCallback;
}