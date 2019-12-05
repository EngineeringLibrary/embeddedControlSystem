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
    operationalInput  = 0;
    operationalOutput = 0;
    startIterator = true;
}

template <typename Type>
void ControlHandler::systemLoopHandler<Type>::startLoop(void (*loopFunction2Call)(void*)){
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
void ControlHandler::systemLoopHandler<Type>::stopLoop(){
    timer_pause(timer_group, timer_idx);
    timer_group_intr_disable( timer_group, TIMG_T0_INT_ENA_M);
    timer_disable_intr(timer_group, timer_idx);
}

template <typename Type>
void ControlHandler::systemLoopHandler<Type>::pauseLoop(){
    // timer_pause(timer_group, timer_idx);
    startIterator = false;
}

template <typename Type>
void ControlHandler::systemLoopHandler<Type>::resumeLoop(){
    // timer_start(timer_group, timer_idx);
    startIterator = true;
}

void IRAM_ATTR ControlHandler::systemLoop(void *para){// timer group 0, ISR
    ControlHandler::systemLoopHandler<long double> *idStructure = ((ControlHandler::systemLoopHandler<long double>*) para);
    timer_idx_t timer_idx = idStructure->timer_idx;
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    if((intr_status & BIT(timer_idx)) ) {
        TIMERG0.hw_timer[timer_idx].update = 1;
        TIMERG0.int_clr_timers.t0 = 1;
        TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;

        if(idStructure->startIterator)
            switch (idStructure->loopHandler)
            {
            case IDENTIFICATION:
                ControlHandler::systemExitationForIdentificationLoop(idStructure);
                break;

            case RELAY:
                ControlHandler::systemExitationforRelayLoop(idStructure);
                break;

            case CONTROL:
                ControlHandler::systemControlLoop(idStructure);
                break;

            default:
                break;
            }
    }
}

template <typename Type>
void ControlHandler::systemExitationForIdentificationLoop(ControlHandler::systemLoopHandler<Type> *idStructure){// timer group 0, ISR
    if(idStructure->iterator < idStructure->maxIterator){
        idStructure->signal[idStructure->channel]->setPowerLevel(idStructure->reference);
        idStructure->in[idStructure->iterator] = idStructure->reference;
        idStructure->out[idStructure->iterator] = idStructure->signal[idStructure->channel]->getFeedbackForPowerControl();
    
        idStructure->iterator++;
        idStructure->minLimit = 9;
    }     
}

template <typename Type>
void ControlHandler::systemExitationforRelayLoop(ControlHandler::systemLoopHandler<Type> *idStructure){// timer group 0, ISR
    if(idStructure->iterator < idStructure->maxIterator){
        idStructure->signal[idStructure->channel]->setPowerLevel(idStructure->inputSignal);
        idStructure->in [idStructure->iterator] = idStructure->inputSignal - idStructure->operationalInput;
        idStructure->out[idStructure->iterator] = idStructure->signal[idStructure->channel]->getFeedbackForPowerControl() - idStructure->operationalOutput;
        idStructure->error = idStructure->reference - idStructure->out[idStructure->iterator];
        if(fabs(idStructure->error) > idStructure->tolerance && idStructure->error > 0) idStructure->inputSignal = idStructure->maxLimit;
        if(fabs(idStructure->error) > idStructure->tolerance && idStructure->error < 0) idStructure->inputSignal = idStructure->minLimit;
        if(fabs(idStructure->error) < idStructure->tolerance && idStructure->error < 0) idStructure->inputSignal = idStructure->minLimit;
        if(fabs(idStructure->error) < idStructure->tolerance && idStructure->error < 0) idStructure->inputSignal = idStructure->minLimit;
    
        idStructure->iterator++;
        idStructure->minLimit = 10;
    }     
}

template <typename Type>
void ControlHandler::systemControlLoop(ControlHandler::systemLoopHandler<Type> *idStructure){// timer group 0, ISR
    if(idStructure->iterator < idStructure->maxIterator){
        idStructure->out[idStructure->iterator] = idStructure->signal[0]->getFeedbackForPowerControl() - idStructure->operationalOutput;
        idStructure->in[idStructure->iterator] = idStructure->pid[0]->OutputControl( idStructure->reference, idStructure->out[idStructure->iterator]) + idStructure->operationalInput;
        idStructure->signal[0]->setPowerLevel( idStructure->in[idStructure->iterator]);
    
        idStructure->iterator++;
        idStructure->minLimit = 11;
    }  
}

template <typename Type>
void ControlHandler::controlLoop(ControlHandler::systemLoopHandler<Type> *idStructure){// timer group 0, ISR
    idStructure->out[idStructure->iterator] = idStructure->signal[idStructure->channel]->getFeedbackForPowerControl();
    idStructure->in[idStructure->iterator]  = idStructure->pid[0]->OutputControl(
                                                                        idStructure->reference, 
                                                                        idStructure->out[idStructure->iterator]);
    idStructure->signal[idStructure->channel]->setPowerLevel(idStructure->in[idStructure->iterator]);
}

template <typename Type>
void ControlHandler::normalController(ControlHandler::systemLoopHandler<Type> *idStructure, uint_fast8_t repetition)//verificar a estabilidade para vários canais, se pvParameter é diferente
{
    for(uint_fast8_t i = 0; i < repetition; ++i)
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
}

template <typename Type>
void ControlHandler::wifiSend(ControlHandler::systemLoopHandler<Type> *idStructure)//verificar a estabilidade para vários canais, se pvParameter é diferente
{
    std::stringstream ss; ss << std::setw(2*5+1) << std::setprecision(5) << std::fixed;
    uint16_t counter = 0;
    for(uint16_t k = 0; k < idStructure->iterator/50; ++k){
        for(uint16_t j = 0; j < 50; ++j){
            if(counter > idStructure->maxIterator){
                k = idStructure->maxIterator/50; break;
            }
            ss << idStructure->in[counter] << "   " << idStructure->out[counter] << "\n"; 
            counter++;
        }
        idStructure->wifi << ss.str();ss.clear();
        ets_delay_us(100000);
    }
    idStructure->wifi << ss.str(); ss.clear();
    idStructure->iterator = 0;
}

template <typename Type>
void ControlHandler::reTune(systemLoopHandler<Type> *idStructure)
{
    for(unsigned j = idStructure->operationalPointIterator; j < idStructure->iterator; ++j)
        idStructure->rls[idStructure->channel]->optimize(idStructure->in[j], idStructure->out[j]);

    LinAlg::Matrix<long double> FOP = c2dConversion(idStructure->boost[idStructure->channel][0]); //etapa de sintonia
    idStructure->pid[0][0] = ControlHandler::controllerTuning(FOP,"PI", ControlHandler::tune[0]);
    std::cout << idStructure->pid[0]->getParams() << std::endl;
    idStructure->wifi << idStructure->boost[idStructure->channel]->print();
    //idStructure->wifi << idStructure->pid[0]->getParams();
}


void ControlHandler::relayExitationLoop(void* pvParameter)
{
    ControlHandler::systemLoopHandler<long double> *idStructure = ((ControlHandler::systemLoopHandler<long double>*) pvParameter);
    std::stringstream ss; ss << std::setw(2*5+1) << std::setprecision(5) << std::fixed << "\nEntrada | Saida \n";  idStructure->wifi << ss.str(); ss.clear();
    idStructure->in = new int16_t[idStructure->maxIterator]; idStructure->out = new int16_t[idStructure->maxIterator];
    idStructure->iterator = 0;
    
   
    idStructure->reference = (idStructure->maxLimit + idStructure->minLimit)/2;
    idStructure->loopHandler = IDENTIFICATION;
    idStructure->startLoop(ControlHandler::systemLoop); // esse primeiro loop objetiva estabilizar a saída no ponto de operação
    adc1_get_raw((adc1_channel_t)idStructure->channel);
    ControlHandler::normalController(idStructure,10);
    idStructure->operationalPointIterator = idStructure->iterator - 1;
    idStructure->operationalInput  =  idStructure->in [idStructure->operationalPointIterator]; 
    idStructure->operationalOutput =  idStructure->out[idStructure->operationalPointIterator]; 

    std::cout << idStructure->minLimit << std::endl;

    idStructure->loopHandler = RELAY;
    idStructure->reference = idStructure->signal[0]->getSignalBehavior("ccLevel");
    ControlHandler::normalController(idStructure,40);
    idStructure->signal[idStructure->channel]->setPowerLevel(0);
    gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  0);
    gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 0);
    idStructure->pauseLoop();

    std::cout << idStructure->minLimit << std::endl;

    ControlHandler::wifiSend(idStructure);
    ControlHandler::reTune(idStructure);

    idStructure->loopHandler = CONTROL;
    idStructure->resumeLoop(); // Etapa de controle
    adc1_get_raw((adc1_channel_t)idStructure->channel); bool flag = false;
    for(unsigned j = 0; j < 25; ++j){
        for(unsigned j = 0; j < 4; ++j){
            gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 0); ets_delay_us(100);
            gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  1); ets_delay_us(50);

            gpio_set_level(idStructure->signal[0]->getOutputHandlerDirectPin(),  0); ets_delay_us(100);
            gpio_set_level(idStructure->signal[0]->getOutputHandlerReversePin(), 1); ets_delay_us(50);
        }
        if(flag) idStructure->reference = idStructure->minLimit;
        else     idStructure->reference = idStructure->maxLimit;

        flag = !flag;
    }
    idStructure->stopLoop();

    std::cout << idStructure->minLimit << std::endl;

    ControlHandler::wifiSend(idStructure);
                                                                       
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

void ControlHandler::squaredWaveExitationLoop(void* pvParameter)
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);
    uint32_t minLimit = idStructure->minLimit, maxLimit = idStructure->maxLimit;
    idStructure->iterator = 0;
    std::cout << "Entrou 1\n";
    idStructure->loopHandler = IDENTIFICATION;
    idStructure->startLoop(ControlHandler::systemLoop);
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


void ControlHandler::closedLoopTwoFaseNormalController(void* pvParameter)
{
    ControlHandler::systemLoopHandler<uint32_t> *idStructure = ((ControlHandler::systemLoopHandler<uint32_t>*) pvParameter);

    uint32_t minLimit = idStructure->minLimit, maxLimit = idStructure->maxLimit;
    idStructure->iterator = 0;
    std::cout << "Entrou 1\n";

    idStructure->loopHandler = CONTROL;
    idStructure->startLoop(ControlHandler::systemLoop); // Lembrar que quando fr chamar mais de um timer e quando for destruir essa função provavelmente vai ocorrer um erro
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