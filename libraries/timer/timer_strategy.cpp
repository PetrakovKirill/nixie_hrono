#include "timer_strategy.h"


#define ARRAY_QUANTITY(array)      (sizeof(array) / (sizeof(*array)))



timerStrategy :: timerStrategy(void) {
    memset(timerList, 0x00, sizeof(timerList));
}



void timerStrategy :: AddTimer(timer *pTimer) {
    uint8_t i;
    for (i = 0; i < ARRAY_QUANTITY(timerList) ; ++i) {
        if (timerList[i] == NULL){
            break;
        }
    }

    if (i < ARRAY_QUANTITY(timerList)) {
        timerList[i] = pTimer;
    }
}



void timerStrategy :: RemoveTimer(timer *pTimer) {
    uint8_t i;
    for (i = 0; i < ARRAY_QUANTITY(timerList) ; ++i) {
        if (timerList[i] == pTimer){
            timerList[i] = NULL;
        }
    }
}



void timerStrategy :: Tick(void) {
    uint8_t i;
    for (i = 0; i < ARRAY_QUANTITY(timerList) ; ++i) {
        if (timerList[i]) {
            timerList[i]->Tick();
        }
    }
}