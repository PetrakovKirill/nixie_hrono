#include "timer.h"
#include <stddef.h>

timer::timer(event *handler = NULL, uint32_t ticksInterval = 0) {
    ChangeEvent(handler);
    SetInterval(ticksInterval);
}



void timer::SetInterval(uint32_t ticksInterval) {
    interval = ticksInterval;
}



uint32_t timer::GetInterval(void) {
    return (interval);
}



void timer::Start(void) {
    active = true;
}



void timer::Stop(void) {
    active = false;
}



void timer::Tick(void) {
    if (active) {
        cnt -= (cnt) ? (1) : (0);
        if (cnt == 0) {
            Stop();
            if (eventHandler) {
                eventHandler->EventHandler();
            }
        }
    }
}



void timer::ResetCounter(void) {
    cnt = interval;
}


void timer::ChangeEvent(event *handler) {
    eventHandler = handler;
}