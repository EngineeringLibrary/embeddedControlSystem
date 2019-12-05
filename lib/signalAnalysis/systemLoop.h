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
#include "pidTuning.h"

namespace ControlHandler{
    typedef enum {
        IDENTIFICATION = 0, /*!<Hw timer group 0*/
        RELAY = 1, /*!<Hw timer group 1*/
        CONTROL = 2,
        LOOP_MAX,
    } loopHandler_t;


    template <typename Type>
    struct systemLoopHandler{

        systemLoopHandler();
        
        void startLoop(void (*loopFunction2Call)(void*));
        void stopLoop();
        void pauseLoop();
        void resumeLoop();

        volatile int16_t inputSignal, minLimit, maxLimit, channel, tolerance, operationalInput, operationalOutput;
        Type reference, error;
        ControlHandler::PID<Type> **pid;
        ModelHandler::ARX<Type> **boost;
        ElectroStimulation::bioSignalController **signal;
        OptimizationHandler::RecursiveLeastSquare<Type> **rls;
        volatile timer_group_t timer_group;
        volatile timer_idx_t timer_idx;
        timer_config_t config;
        volatile float TIMER_SCALE, TIMER_FINE_ADJ, TIMER_INTERVAL0_SEC;
        int16_t *in, *out;
        volatile uint16_t iterator, maxIterator, operationalPointIterator;
        bool startIterator;
        Communication::Wifi wifi;
        TaskHandle_t *xHandle;
        loopHandler_t loopHandler;
    };
    

    //template <typename Type>
    //static void closedLoopNormalController(void*);

    //template <typename Type>
    //static void closedLoopTwoFaseNormalController(void*){}

    static void squaredWaveExitationLoop(void*);

    static void relayExitationLoop(void*);

    static void closedLoopTwoFaseNormalController(void*);
    
    //template <typename Type>
    void IRAM_ATTR systemLoop(void *para);

    template <typename Type>
    inline void systemControlLoop(systemLoopHandler<Type> *idStructure);

    template <typename Type>
    inline void systemExitationForIdentificationLoop(systemLoopHandler<Type> *idStructure);

    template <typename Type>
    inline void systemExitationforRelayLoop(systemLoopHandler<Type> *idStructure);

    template <typename Type>
    inline void controlLoop(systemLoopHandler<Type> *idStructure);
    
    template <typename Type>
    inline void normalController (systemLoopHandler<Type> *idStructure, uint_fast8_t repetition);

    template <typename Type>
    inline void wifiSend(systemLoopHandler<Type> *idStructure);

    template <typename Type>
    inline void reTune(systemLoopHandler<Type> *idStructure);

}

#include "systemLoop.hpp"
#endif // SYSTEMLOOP_H