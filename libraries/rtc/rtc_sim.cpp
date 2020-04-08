#include "rtc_sim.h"
#include <stddef.h>

#define SEC_24_HOURS    (86400)
#define SEC_1_HOUR      ( 3600)
#define SEC_1_MIN       (   60)


rtcSim :: rtcSim(timerStrategy *strategy) : 
                timHandler(this), tim(&timHandler, 1000) {
    timStrategy = strategy;
    timStrategy->AddTimer(&tim);
    timeCount = 0;

    tim.Start();
}


/* Пустышка, текущая реализация не требует чтения данных
 * с внешнего модуля */
void rtcSim :: ReadTime(void) { }



uint32_t rtcSim :: GetTime(void) { 
    return (timeCount); 
}



void rtcSim :: SetTime(uint32_t time) {
    timeCount = time;
}


void rtcSim :: timerEvent :: EventHandler(void) {
    inst->timeCount++;
    inst->tim.ResetCounter();
    inst->tim.Start();
}