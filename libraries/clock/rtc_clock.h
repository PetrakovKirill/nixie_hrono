#ifndef _RTC_CLOCK_IMPEMENTATION_H_
#define _RTC_CLOCK_IMPEMENTATION_H_

#include "clock.h"
#include "display.h"
#include "rtc.h"

class rtcClock : public clock {
public:
    rtcClock(display *pDisplay, rtc *pRtc);
    void ShowTime(void);
    void SetTime(uint32_t seconds);
    uint32_t GetTime(void);

private:
    rtc     *time;
    display *displ;
};

#endif /* _RTC_CLOCK_IMPEMENTATION_H_ */