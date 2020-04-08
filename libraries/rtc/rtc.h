#ifndef _RTC_INTERFACE_H_
#define _RTC_INTERFACE_H_

#include <stdint.h>

class rtc {
public:
    virtual void      ReadTime(void)         = 0;
    virtual uint32_t  GetTime(void)          = 0;
    virtual void      SetTime(uint32_t time) = 0;
};

#endif /* _RTC_INTERFACE_H_ */