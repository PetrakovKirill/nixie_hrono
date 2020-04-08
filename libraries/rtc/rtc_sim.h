#ifndef _RTC_SIMULATION_H_
#define _RTC_SIMULATION_H_

#include "rtc.h"
#include "timer_strategy.h"
#include "event.h"


class rtcSim : public rtc {
public:
    rtcSim(timerStrategy *strategy);
    void      ReadTime(void);
    uint32_t  GetTime(void);
    void      SetTime(uint32_t time);

private:
    class timerEvent : public event {
    public:
        timerEvent(rtcSim *pInst) : inst(pInst) {}
        void EventHandler(void);
    private:
        rtcSim *inst;
    };

    friend timerEvent;

    // friend void RtcSim_TimerHandler(void *pUserData);
    // inline void CounterIncremet(void);

    timerEvent         timHandler;
    timerStrategy     *timStrategy;
    timer              tim;
    uint32_t           timeCount;
};

#endif /* _RTC_SIMULATION_H_ */

// 86 400 - 24 часа
// 3 600  - 1 час
// 60     - 1 минута