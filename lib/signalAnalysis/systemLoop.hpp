#include "systemLoop.h"

template <typename Type>
ControlHandler::systemLoopHandler<Type>::systemLoopHandler(){
    iterator = 0;
    in = NULL;
    out = NULL;
    reference           = 0;
    timer_idx           = TIMER_0;
    timer_group         = TIMER_GROUP_0; /*!< Test on timer group 0 */
    config.alarm_en     = 1;
    config.auto_reload  = 1;
    config.counter_dir  = TIMER_COUNT_UP;
    config.divider      = 8000;/*!< Hardware timer clock divider, 80 to get 1MHz clock to timer */
    config.intr_type    = TIMER_INTR_LEVEL; /*!< Timer level interrupt */
    config.counter_en   = TIMER_PAUSE;
    TIMER_SCALE         = (TIMER_BASE_CLK / config.divider);  /*!< used to calculate counter value */
    TIMER_FINE_ADJ      =  (0*(TIMER_BASE_CLK / config.divider)/1000000); /*!< used to compensate alarm value */
    TIMER_INTERVAL0_SEC = (0.001);   /*!< test interval for timer 0 */
}

template <typename Type>
void ControlHandler::systemLoopHandler<Type>::startLoop(void (*loopFunction2Call)(void*))
{
    /*Configure timer*/
    timer_init(timer_group, timer_idx, &config);
    /*Stop timer this*/
    timer_pause(timer_group, timer_idx);
    /*Load counter value */
    timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);
    /*Set alarm value*/
    timer_set_alarm_value(timer_group, timer_idx, (TIMER_INTERVAL0_SEC * TIMER_SCALE) - TIMER_FINE_ADJ);
    /*Enable timer interrupt*/
    timer_enable_intr(timer_group, timer_idx);
    /*Set ISR handler*/
    timer_isr_register(timer_group, timer_idx, loopFunction2Call, (void*) this, ESP_INTR_FLAG_IRAM, NULL);
    /*Start timer counter*/
    timer_start(timer_group, timer_idx);
}

//template <typename Type>
void IRAM_ATTR ControlHandler::systemControlLoop(void *para){// timer group 0, ISR
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) para);
    timer_idx_t timer_idx = idStructure->timer_idx;
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    if((intr_status & BIT(timer_idx))) {
        TIMERG0.hw_timer[timer_idx].update = 1;
        TIMERG0.int_clr_timers.t0 = 1;
        TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;

        //idStructure->signal->setPowerLevel( 
        //                        idStructure->pid->OutputControl(
        //                                            idStructure->reference,
        //                                            idStructure->signal->getFeedbackForPowerControl()));
        idStructure->signal->setPowerLevel(idStructure->reference);
    }
}

void ControlHandler::closedLoopNormalController(void* pvParameter)//verificar a estabilidade para vários canais, se pvParameter é diferente
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);
    ElectroStimulation::bioSignalController *signal = idStructure->signal;   
    uint32_t time = (1000000/signal->getSignalBehavior("freq")) - signal->getSignalBehavior("period");
    
    while(1){
       
        for(uint_fast8_t i = 0; i < 10; ++i)
        {
            gpio_set_level(signal->getOutputHandlerDirectPin(), 0);
            ets_delay_us(signal->getSignalBehavior("period"));
            gpio_set_level(signal->getOutputHandlerDirectPin(), 1);
            ets_delay_us(time);
            
            if(idStructure->reference > 100)
                idStructure->reference = 0;
            idStructure->reference++;
        }
        //std::cout << reference << ", " << y << ", " << u << std::endl;
    }
}

void IRAM_ATTR ControlHandler::systemExitationForIdentificationLoop(void *para){// timer group 0, ISR
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) para);
    timer_idx_t timer_idx = idStructure->timer_idx;
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    if((intr_status & BIT(timer_idx))) {
        TIMERG0.hw_timer[timer_idx].update = 1;
        TIMERG0.int_clr_timers.t0 = 1;
        TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;

        if(idStructure->iterator < 2000 && idStructure->in && idStructure->out){
            idStructure->signal->setPowerLevel(idStructure->reference);
            idStructure->in[idStructure->iterator] = idStructure->reference;
            idStructure->out[idStructure->iterator] = idStructure->signal->getFeedbackForPowerControl();
        
            idStructure->iterator++;
        }          
    }
}

void ControlHandler::squaredWaveExitationLoop(void* pvParameter)
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);
    uint32_t minLimit = 10, maxLimit = 80;
    idStructure->in = new uint32_t[2000]; idStructure->out = new uint32_t[2000];
    idStructure->iterator = 0;

    for(unsigned j = 0; j < 10; ++j)
    {
        idStructure->reference = minLimit;
        ets_delay_us(500000);

        idStructure->reference = maxLimit;
        ets_delay_us(500000);
    }

    std::cout << "entrou 2\n";


    std::stringstream ss; ss << std::setw(2*5+1) << std::setprecision(5) << std::fixed << "\nEntrada | Saida \n";
    std::cout << "entrou 3\n";
    for(unsigned j = 0; j < 2000; ++j)
    {
        ss << idStructure->in[j] << "   " << idStructure->out[j] << "\n";
    }
    std::cout << "entrou 4\n";
    idStructure->wifi << ss.str();
    std::cout << "entrou 5\n";

    while(1);
}

void ControlHandler::relayExitationLoop(void* pvParameter)
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);
    uint32_t minLimit = 10, maxLimit = 80, lastInputValue = minLimit;
    idStructure->in = new uint32_t[2000]; idStructure->out = new uint32_t[2000];
    idStructure->iterator = 0;
    int32_t e; uint32_t tolerance = 100;

    for(unsigned j = 0; j < 10; ++j)
    {
        e = idStructure->reference - idStructure->signal->getFeedbackForPowerControl();
        if(abs(e) > tolerance && e > 0) idStructure->inputSignal = maxLimit;
        if(abs(e) > tolerance && e < 0) idStructure->inputSignal = minLimit;
        if(abs(e) < tolerance && e < 0) idStructure->inputSignal = minLimit;
        if(abs(e) < tolerance && e < 0) idStructure->inputSignal = minLimit;

        idStructure->reference = minLimit;
    }

    std::cout << "entrou 2\n";


    std::stringstream ss; ss << std::setw(2*5+1) << std::setprecision(5) << std::fixed << "\nEntrada | Saida \n";
    std::cout << "entrou 3\n";
    for(unsigned j = 0; j < 2000; ++j)
    {
        ss << idStructure->in[j] << "   " << idStructure->out[j] << "\n";
    }
    std::cout << "entrou 4\n";
    idStructure->wifi << ss.str();
    std::cout << "entrou 5\n";

    while(1);
}