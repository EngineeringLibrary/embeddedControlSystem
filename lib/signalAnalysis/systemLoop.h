#ifndef SYSTEMLOOP_H
#define SYSTEMLOOP_H

#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#include "bioSignalGenerator.h"
#include "arx.h"
#include "pid.h"

namespace ControlHandler{
    template <typename Type>
    struct systemLoopHandler{

        systemLoopHandler();
        
        void startLoop(void (*loopFunction2Call)(void*));
        void stopLoop();

        volatile uint32_t reference, inputSignal, minLimit, maxLimit, channel, tolerance;
        ControlHandler::PID<Type> **pid;
        ModelHandler::ARX<double> **boost;
        ElectroStimulation::bioSignalController **signal;
        OptimizationHandler::RecursiveLeastSquare<double> **rls;
        volatile timer_group_t timer_group;
        volatile timer_idx_t timer_idx;
        timer_config_t config;
        volatile float TIMER_SCALE, TIMER_FINE_ADJ, TIMER_INTERVAL0_SEC;
        Type *in, *out;
        volatile uint32_t iterator, maxIterator;
        Communication::Wifi wifi;
        TaskHandle_t *xHandle;
    };
    

    //template <typename Type>
    static void closedLoopNormalController(void*);

    template <typename Type>
    static void closedLoopTwoFaseNormalController(void*){}

    static void squaredWaveExitationLoop(void*);

    static void relayExitationLoop(void*);

    static void closedLoopTwoFaseNormalController(void*);
    
    //template <typename Type>
    void IRAM_ATTR systemControlLoop(void *para);

    void IRAM_ATTR systemExitationForIdentificationLoop(void *para);

    void IRAM_ATTR systemExitationforRelayLoop(void *para);

    void IRAM_ATTR controlLoop(void *para);

}

#include "systemLoop.hpp"
#endif // SYSTEMLOOP_H