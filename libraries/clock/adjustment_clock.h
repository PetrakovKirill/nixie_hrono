#ifndef _ADJUSTMENT_CLOCK_IMPLEMENTATION_H_
#define _ADJUSTMENT_CLOCK_IMPLEMENTATION_H_

#include "clock.h"
#include "display.h"

class adjustmentClock : public clock {
public:
    adjustmentClock(display *pDisplay);
    void ShowTime(void);
    void SetTime(uint32_t seconds);
    uint32_t GetTime(void);

    void AdjustmentHour(void);
    void AdjustmentMinutes(void);

    void Increment(void);
    void Decrement(void);

private:
    const char *fmtParam;

    display *displ;
    uint32_t delta;
    uint32_t time;
};

#endif /* _ADJUSTMENT_CLOCK_IMPLEMENTATION_H_ */