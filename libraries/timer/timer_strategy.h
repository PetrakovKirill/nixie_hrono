#ifndef _TIMER_STRATEGY_H_
#define _TIMER_STRATEGY_H_

#include <string.h>
#include <timer.h>
#include <stddef.h>

#if defined(TIMER_STRATEGY_QUANTITY) > 0
#define QUANTITY    (TIMER_STRATEGY_QUANTITY)
#else 
#define QUANTITY    (10)
#endif /* TIMER_STRATEGY_QUANTITY */


class timerStrategy {
public:
    timerStrategy(void);
    void AddTimer(timer *pTimer);
    void RemoveTimer(timer *pTimer);
    void Tick(void);

private:
    timer *timerList[QUANTITY];
};

#endif /* _TIMER_STRATEGY_H_ */