#ifndef _TIMER_ABSTRACT_INTERFACE_H_
#define _TIMER_ABSTRACT_INTERFACE_H_

#include <stdint.h>
#include "event.h"

class timer {
public:
    timer(event *handler, uint32_t interval);
    void     SetInterval(uint32_t ticks);
    uint32_t GetInterval(void);
    void     ChangeEvent(event *handler);
    void     Start(void);
    void     Stop(void);
    void     Tick(void);
    void     ResetCounter(void);

private:
    event   *eventHandler;
    uint32_t interval;
    uint32_t cnt;
    bool     active;
};

#endif /* _TIMER_ABSTRACT_INTERFACE_H_ */



