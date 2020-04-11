#include "rtc_clock.h"
#include <stdio.h>


#define SEC_24_HOURS    (86400)
#define SEC_1_HOUR      ( 3600)
#define SEC_1_MIN       (   60)


rtcClock :: rtcClock(display *pDisplay, rtc *pRtc) {
    displ = pDisplay;
    time  = pRtc;
}



void rtcClock :: ShowTime(void) {
    char            printString[5] = {0};
    uint32_t        currentTime;
    uint16_t        hour, min;
    
    time->ReadTime();

    currentTime = time->GetTime();
    hour = (currentTime % SEC_24_HOURS) / SEC_1_HOUR;
    min  = (currentTime % SEC_1_HOUR)   / SEC_1_MIN;
    snprintf(printString, sizeof(printString), "%02u%02u", 
                                hour, min);

    displ->Print(printString);
}



void rtcClock :: SetTime(uint32_t seconds) {
    time->SetTime(seconds);
}



uint32_t rtcClock :: GetTime(void) {
    return (time->GetTime());
}