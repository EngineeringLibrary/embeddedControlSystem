#include "bioSignalGenerator.h"

void ElectroStimulation::bioSignalController::powerControllerInit(const gpio_num_t &pin, const uint32_t &freq, const ledc_channel_t &channel)
{
    gpio_pad_select_gpio((gpio_num_t)pin);
    gpio_set_direction((gpio_num_t)pin, GPIO_MODE_OUTPUT); 

    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.freq_hz = freq;
    ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_0;
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

	ledc_channel.duty       = 0; 
	ledc_channel.channel    = channel;
	ledc_channel.gpio_num   = pin;
	ledc_channel.timer_sel  = LEDC_TIMER_0;
	ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel_config(&ledc_channel);
}

void ElectroStimulation::bioSignalController::setPowerLevel(const double &powerLevel)
{
    ledc_channel.duty       = (uint16_t)((100-powerLevel)*1024/100); 
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, ledc_channel.duty);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}

void ElectroStimulation::bioSignalController::setOutputHandlerPin(const gpio_num_t &outputHandlerPin)
{
    this->outputHandlerPin = outputHandlerPin;
    gpio_pad_select_gpio(this->outputHandlerPin);
    gpio_set_direction(this->outputHandlerPin, GPIO_MODE_OUTPUT); 
}

void ElectroStimulation::bioSignalController::addSignalBehavior(const std::string &signalBehaviorName, const double &signalBehavior)
{
    this->signalBehaviorHandler.emplace(signalBehaviorName, signalBehavior);
}

void ElectroStimulation::bioSignalController::removeSignalBehavior(const std::string &signalBehaviorName)
{
    this->signalBehaviorHandler.erase (signalBehaviorName);
}

double ElectroStimulation::bioSignalController::getSignalBehavior(const std::string &signalBehavior) const
{
    return this->signalBehaviorHandler.find(signalBehavior)->second;                        
}

void ElectroStimulation::burstController(void* pvParameter)
{
    bioSignalController signalHandler = *((bioSignalController*) pvParameter);
    uint16_t Ton = signalHandler.getSignalBehavior("Ton"), period = signalHandler.getSignalBehavior("period");

    while (1)
    {
        for(unsigned i = 0; i < 50; ++i)
        {
            signalHandler.setPowerLevel(i);
            for(unsigned j = 0; j < 100; ++j)
            {
                gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
                ets_delay_us(Ton);
                gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(),1);
                ets_delay_us(period-Ton);
            }
        }

        for(unsigned i = 50; i > 0; --i)
        {
            signalHandler.setPowerLevel(i);
            for(unsigned j = 0; j < 100; ++j)
            {
                gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(),0);
                ets_delay_us(Ton);
                gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(),1);
                ets_delay_us(period-Ton);
            }
        }
    }

}