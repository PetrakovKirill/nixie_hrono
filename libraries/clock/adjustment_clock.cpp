#include "adjustment_clock.h"
#include "stdio.h"

#define SEC_24_HOURS    (86400)
#define SEC_1_HOUR      ( 3600)
#define SEC_1_MIN       (   60)

const static char *adjHour = "%02u  ";
const static char *adjMin  = "  %02u";


adjustmentClock :: adjustmentClock(display *pDisplay) {
    displ = pDisplay;
    time  = 0;
    AdjustmentHour();
}



void adjustmentClock :: ShowTime(void) {
    char     printString[5] = {0};
    uint32_t param;

    if (fmtParam == adjHour) {
        param = (time % SEC_24_HOURS) / SEC_1_HOUR;
    } else {
        param = (time % SEC_1_HOUR)   / SEC_1_MIN;
    }

    snprintf(printString, sizeof(printString), fmtParam, param);

    displ->Print(printString);
}



void adjustmentClock :: SetTime(uint32_t seconds) {
    time = seconds;
}



uint32_t adjustmentClock :: GetTime(void) {
    return (time);
}



void adjustmentClock :: AdjustmentHour(void) {
    fmtParam = adjHour;
    delta    = SEC_1_HOUR; 
}



void adjustmentClock :: AdjustmentMinutes(void) {
    fmtParam = adjMin;
    delta    = SEC_1_MIN;
}



void adjustmentClock :: Increment(void) {
    time += delta;
}



void adjustmentClock :: Decrement(void) {
    time -= delta;
}