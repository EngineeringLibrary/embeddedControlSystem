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
    config.divider      = 3000;/*!< Hardware timer clock divider, 80 to get 1MHz clock to timer */
    config.intr_type    = TIMER_INTR_LEVEL; /*!< Timer level interrupt */
    config.counter_en   = TIMER_PAUSE;
    TIMER_SCALE         = (TIMER_BASE_CLK / config.divider);  /*!< used to calculate counter value */
    TIMER_FINE_ADJ      =  (0*(TIMER_BASE_CLK / config.divider)/1000000); /*!< used to compensate alarm value */
    TIMER_INTERVAL0_SEC = (0.0001);   /*!< test interval for timer 0 */
    maxIterator = 600;
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

template <typename Type>
void ControlHandler::systemLoopHandler<Type>::stopLoop()
{
    timer_pause(timer_group, timer_idx);
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
        idStructure->signal[idStructure->channel]->setPowerLevel(idStructure->reference);
    }
}

void ControlHandler::closedLoopNormalController(void* pvParameter)//verificar a estabilidade para vários canais, se pvParameter é diferente
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);
    ElectroStimulation::bioSignalController *signal = idStructure->signal[idStructure->channel];   
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

        if(idStructure->iterator < idStructure->maxIterator){
            idStructure->signal[idStructure->channel]->setPowerLevel(idStructure->reference);
            idStructure->in[idStructure->iterator] = idStructure->reference;
            idStructure->out[idStructure->iterator] = idStructure->signal[idStructure->channel]->getFeedbackForPowerControl();
        
            idStructure->iterator++;
        }     

    }
}

void ControlHandler::squaredWaveExitationLoop(void* pvParameter)
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);
    uint32_t minLimit = idStructure->minLimit, maxLimit = idStructure->maxLimit;
    idStructure->iterator = 0;
    std::cout << "Entrou 1\n";
    idStructure->startLoop(ControlHandler::systemExitationForIdentificationLoop);
    adc1_get_raw((adc1_channel_t)idStructure->channel); // Criar função ADCReconfigure dentro do biosignalgenerator
    for(uint_fast8_t j = 0; j < 10; ++j)
    {
        idStructure->reference = minLimit;
        ets_delay_us(30000);

        idStructure->reference = maxLimit;
        ets_delay_us(30000);
    }
    idStructure->stopLoop();
    

    std::cout << "entrou 2\n";


    std::stringstream ss; ss << std::setw(2*5+1) << std::setprecision(5) << std::fixed << "\nEntrada | Saida \n";
    std::cout << "entrou 3\n";
    uint16_t counter = 0;
    for(unsigned k = 0; k < idStructure->maxIterator/50; ++k){
        for(unsigned j = 0; j < 50; ++j){
            if(counter > idStructure->maxIterator){
                k = idStructure->maxIterator/50; break;
            }
            ss << idStructure->in[counter] << "   " << idStructure->out[counter] << "\n"; 
            counter++;
        }
        idStructure->wifi << ss.str();ss.clear();
        ets_delay_us(100000);
    }
    std::cout << "entrou 5\n";

    for(unsigned j = 0; j < counter; ++j){
        idStructure->rls[idStructure->channel]->optimize(idStructure->in[j], idStructure->out[j]);
    }

    idStructure->wifi << idStructure->boost[idStructure->channel]->print();

    if(idStructure->xHandle[idStructure->channel] != NULL){
        std::cout << "EntrouS 1\n";
        delete idStructure->in; delete idStructure->out; 
        std::cout << "EntrouS 2\n";
        idStructure->in = NULL; idStructure->out = NULL;
        std::cout << "EntrouS 3\n";
		vTaskDelete(idStructure->xHandle[idStructure->channel]);
        std::cout << "EntrouS 4\n";
    }
}

void IRAM_ATTR ControlHandler::systemExitationforRelayLoop(void *para){// timer group 0, ISR
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) para);
    int32_t e; //uint32_t tolerance = 80; uint32_t minLimit = idStructure->minLimit, maxLimit = idStructure->maxLimit;
    timer_idx_t timer_idx = idStructure->timer_idx;
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    if((intr_status & BIT(timer_idx))) {
        TIMERG0.hw_timer[timer_idx].update = 1;
        TIMERG0.int_clr_timers.t0 = 1;
        TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;


        if(idStructure->iterator < idStructure->maxIterator){
            idStructure->signal[idStructure->channel]->setPowerLevel(idStructure->inputSignal);
            idStructure->in [idStructure->iterator] = idStructure->inputSignal;
            idStructure->out[idStructure->iterator] = idStructure->signal[idStructure->channel]->getFeedbackForPowerControl();
            e = idStructure->reference - idStructure->out[idStructure->iterator];
            if(abs(e) > idStructure->tolerance && e > 0) idStructure->inputSignal = idStructure->maxLimit;
            if(abs(e) > idStructure->tolerance && e < 0) idStructure->inputSignal = idStructure->minLimit;
            if(abs(e) < idStructure->tolerance && e < 0) idStructure->inputSignal = idStructure->minLimit;
            if(abs(e) < idStructure->tolerance && e < 0) idStructure->inputSignal = idStructure->minLimit;
        
            idStructure->iterator++;
        }     

    }
}

void ControlHandler::relayExitationLoop(void* pvParameter)
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);
    //uint32_t minLimit = 5, maxLimit = 50, lastInputValue = minLimit;
    idStructure->in = new uint32_t[idStructure->maxIterator]; idStructure->out = new uint32_t[idStructure->maxIterator];
    idStructure->iterator = 0;
    
    idStructure->reference = idStructure->signal[0]->getSignalBehavior("ccLevel");
    
    idStructure->startLoop(ControlHandler::systemExitationForIdentificationLoop); // esse primeiro loop objetiva estabilizar a saída no ponto de operação
    adc1_get_raw((adc1_channel_t)idStructure->channel);
    for(unsigned j = 0; j < 5; ++j)
    {
        gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 0);
        ets_delay_us(100);
        gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  1);
        ets_delay_us(50);

        gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  0);
        ets_delay_us(100);
        gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 1);
        ets_delay_us(50);
    }
    idStructure->stopLoop();

    idStructure->startLoop(ControlHandler::systemExitationforRelayLoop);
    adc1_get_raw((adc1_channel_t)idStructure->channel);
    for(unsigned j = 0; j < 100; ++j)
    {
        gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 0);
        ets_delay_us(100);
        gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  1);
        ets_delay_us(50);

        gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  0);
        ets_delay_us(100);
        gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 1);
        ets_delay_us(50);
    }
    idStructure->stopLoop();
    idStructure->signal[idStructure->channel]->setPowerLevel(0);
    gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  0);
    gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 0);
    std::cout << "entrou 2\n";


    std::stringstream ss; ss << std::setw(2*5+1) << std::setprecision(5) << std::fixed << "\nEntrada | Saida \n";
    std::cout << "entrou 3\n";
    uint16_t counter = 0;
    for(unsigned k = 0; k < idStructure->iterator/50; ++k){
        for(unsigned j = 0; j < 50; ++j){
            if(counter > idStructure->maxIterator){
                k = idStructure->maxIterator/50; break;
            }
            ss << idStructure->in[counter] << "   " << idStructure->out[counter] << "\n"; 
            counter++;
        }
        idStructure->wifi << ss.str();ss.clear();
        ets_delay_us(100000);
    }
    std::cout << "entrou 4\n";
    idStructure->wifi << ss.str();
    std::cout << "entrou 5\n";

    for(unsigned j = 0; j < counter; ++j){
        idStructure->rls[idStructure->channel]->optimize(idStructure->in[j], idStructure->out[j]);
    }

    idStructure->wifi << idStructure->boost[idStructure->channel]->print();

    if(idStructure->xHandle[idStructure->channel] != NULL){
        std::cout << "EntrouS 1\n";
        delete idStructure->in; delete idStructure->out; 
        std::cout << "EntrouS 2\n";
        idStructure->in = NULL; idStructure->out = NULL;
        std::cout << "EntrouS 3\n";
		vTaskDelete(idStructure->xHandle[idStructure->channel]);
        std::cout << "EntrouS 4\n";
    }
}

void IRAM_ATTR ControlHandler::controlLoop(void *para){// timer group 0, ISR
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) para);
    timer_idx_t timer_idx = idStructure->timer_idx;
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    if((intr_status & BIT(timer_idx))) {
        TIMERG0.hw_timer[timer_idx].update = 1;
        TIMERG0.int_clr_timers.t0 = 1;
        TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;
        
        idStructure->out[idStructure->iterator] = idStructure->signal[idStructure->channel]->getFeedbackForPowerControl();
        idStructure->in[idStructure->iterator]  = idStructure->pid[0]->OutputControl(
                                                                            idStructure->reference, 
                                                                            idStructure->out[idStructure->iterator]);
        idStructure->signal[idStructure->channel]->setPowerLevel(idStructure->in[idStructure->iterator]);
    }
}

void ControlHandler::closedLoopTwoFaseNormalController(void* pvParameter)
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);

    uint32_t minLimit = idStructure->minLimit, maxLimit = idStructure->maxLimit;
    idStructure->iterator = 0;
    std::cout << "Entrou 1\n";
    idStructure->startLoop(ControlHandler::controlLoop); // Lembrar que quando fr chamar mais de um timer e quando for destruir essa função provavelmente vai ocorrer um erro
    adc1_get_raw((adc1_channel_t)idStructure->channel); // Criar função ADCReconfigure dentro do biosignalgenerator

    double reference = idStructure->signal[0]->getSignalBehavior("ccLevel");
    uint32_t time = (1000000/idStructure->signal[0]->getSignalBehavior("freq"))-idStructure->signal[0]->getSignalBehavior("period");
    gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  0);
    gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 0);

    while(1){
        //for(uint_fast8_t i = 0; i < 10; ++i)
        //{
        gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 0);
        ets_delay_us(time);
        gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  1);
        ets_delay_us(idStructure->signal[0]->getSignalBehavior("period"));

        gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  0);
        ets_delay_us(time);
        gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 1);
        ets_delay_us(idStructure->signal[0]->getSignalBehavior("period"));
        //}
        std::stringstream ss;
        for(unsigned j = 0; j < 500; ++j){
            ss << idStructure->in[j] << "   " << idStructure->out[j] << "\n"; 
        }
        idStructure->wifi << ss.str();
    }
}