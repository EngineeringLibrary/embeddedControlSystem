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

    while(1){
        signalHandler.setPowerLevel(signalHandler.getSignalBehavior("ccLevel"));
        for(int i = 0; i<10; i++){ 
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
            ets_delay_us(signalHandler.getSignalBehavior("period"));
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 1);
            ets_delay_us(10000-signalHandler.getSignalBehavior("period"));
        }
        ets_delay_us((1000000/signalHandler.getSignalBehavior("freq"))-100000);
    }
}

void ElectroStimulation::normalController(void* pvParameter)
{
    bioSignalController signalHandler = *((bioSignalController*) pvParameter);

    while(1){
        signalHandler.setPowerLevel(signalHandler.getSignalBehavior("ccLevel"));
        gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
        ets_delay_us(signalHandler.getSignalBehavior("period"));
        gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 1);
        ets_delay_us((1000000/signalHandler.getSignalBehavior("freq"))-signalHandler.getSignalBehavior("period"));
    }
}

void ElectroStimulation::modulationController(void* pvParameter)
{
    bioSignalController signalHandler = *((bioSignalController*) pvParameter);
    
    uint16_t time;
    
    while(1){
        signalHandler.setPowerLevel(signalHandler.getSignalBehavior("ccLevel"));
        time = (1000000/signalHandler.getSignalBehavior("freq"));
        for(int i = 0; i < (500000/time); i++ ){
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
            ets_delay_us(time/2); 
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 1);
            ets_delay_us(time/2);
        }
        time = (1000000/(signalHandler.getSignalBehavior("freq")/2));
        for(int i = 0; i < (500000/time); i++ ){
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
            ets_delay_us(time/2); 
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 1);
            ets_delay_us(time/2); 
        }
    }
}

void ElectroStimulation::sd1Controller(void* pvParameter)
{
    bioSignalController signalHandler = *((bioSignalController*) pvParameter);

    uint16_t time, value, valDecay;
    double K = 4/50;
    
    while(1){
        time = (1000000/signalHandler.getSignalBehavior("freq"));
        // duty = signalHandler.getSignalBehavior("period");
        value = signalHandler.getSignalBehavior("ccLevel");
        valDecay = K*(signalHandler.getSignalBehavior("ccLevel")/signalHandler.getSignalBehavior("freq"));
        // dutyDecay = K*(signalHandler.getSignalBehavior("period")/signalHandler.getSignalBehavior("freq"));
        gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
        signalHandler.setPowerLevel(value);
        for(uint32_t i = 0; i < (uint32_t)(5000000/time); ++i){
            //gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
            //ets_delay_us(duty += dutyDecay);
            //gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 1);
            //ets_delay_us(time-duty);
            signalHandler.setPowerLevel(value -= valDecay);
            ets_delay_us(time);
        }
        for(uint32_t i = 0; i < (uint32_t)(5000000/time); ++i){
            //gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
            //ets_delay_us(duty -= dutyDecay);
            //gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 1);
            //ets_delay_us(time-duty);
            signalHandler.setPowerLevel(value += valDecay);
            ets_delay_us(time);
        }
    }
}

void ElectroStimulation::sd2Controller(void* pvParameter)
{
    bioSignalController signalHandler = *((bioSignalController*) pvParameter);

    uint16_t time, duty, value, valDecay, dutyDecay;
    
    while(1){
        time = (1000000/signalHandler.getSignalBehavior("freq"));
        duty = signalHandler.getSignalBehavior("period");
        value = signalHandler.getSignalBehavior("ccLevel");
        valDecay = (7*signalHandler.getSignalBehavior("ccLevel"))/(50*signalHandler.getSignalBehavior("freq"));
        dutyDecay = (7*signalHandler.getSignalBehavior("period"))/(50*signalHandler.getSignalBehavior("freq"));

        signalHandler.setPowerLevel(value);
        for(int i = 0; i < (5000000/time); i++){
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
            ets_delay_us(duty);
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 1);
            ets_delay_us(time-duty);
            duty += dutyDecay;
            value -= valDecay;
            signalHandler.setPowerLevel(value);
        }
        for(int i = 0; i < (5000000/time); i++){
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 0);
            ets_delay_us(duty);
            gpio_set_level((gpio_num_t) signalHandler.getOutputHandlerPin(), 1);
            ets_delay_us(time-duty);
            duty -= dutyDecay;
            value += valDecay;
            signalHandler.setPowerLevel(value);
        }
    }
}